#include <fstream>
#include <math.h>
#include <stdexcept>
#include <iostream>
#include "Nominal.h"
#include <boost/math/distributions/chi_squared.hpp>

Json::Value eigenToJson(Eigen::VectorXd e)
{
    Json::Value json(Json::arrayValue);

    for (int k=0; k<e.size(); k++) {
        json[k] = e[k];
    }

    return json;
}

Eigen::VectorXd jsonToEigenVector(Json::Value j)
{
    Eigen::VectorXd v(j.size());
    int k = 0;
    for (auto n : j) {
        v[k++] = n.asDouble();
    }

    return v;
}

Json::Value eigenToJson(Eigen::MatrixXd e)
{
    Json::Value json(Json::arrayValue);

    int k = 0;
    for (int r=0; r<e.rows(); r++) {
        for (int c=0; c<e.cols(); c++) {
            json[k++] = e(r, c);
        }
    }

    return json;
}

Eigen::MatrixXd jsonToEigenMatrix(Json::Value v)
{
    int s = sqrt(v.size());
    Eigen::MatrixXd m(s, s);

    int k = 0;
    for (int r=0; r<s; r++) {
        for (int c=0; c<s; c++) {
            m(r, c) = v[k++].asDouble();
        }
    }

    return m;
}

double chi2_cdf(double x, double k)
{
    boost::math::chi_squared mydist(k);
    return boost::math::cdf(mydist, x);
}

void Nominal::Bin::add(Eigen::VectorXd v)
{
    values.push_back(v);
}

void Nominal::Bin::compile(int dimensions)
{
    // Updating mean
    mean = Eigen::VectorXd(dimensions);
    for (int k=0; k<dimensions; k++) {
        mean[k] = 0;
    }
    for (auto v : values) {
        mean += v;
    }
    mean /= (double)values.size();

    // Updating cov
    cov = Eigen::MatrixXd(dimensions, dimensions);
    for (int k=0; k<dimensions; k++) {
        for (int l=0; l<dimensions; l++) {
            cov(k, l) = 0;
        }
    }
    for (auto v : values) {
        Eigen::VectorXd d = v - mean;
        cov = cov + d*d.transpose();
    }
    cov = cov/((double)(values.size()-1));
    icov = cov.inverse();
}

    Nominal::Nominal(int dimensions, int nbins)
: dimensions(dimensions),
    nbins(nbins)
{
    for (int k=0; k<nbins; k++) {
        bins[k] = Bin();
    }
}

Nominal::Bin &Nominal::getBin(double t)
{
    int bin = floor(nbins*t);

    return bins[bin];
}

void Nominal::add(double t, Eigen::VectorXd v)
{
    if (v.size() != dimensions) {
        throw std::logic_error("bad dimensions");
    }

    getBin(t).add(v);
}

double Nominal::Bin::chi2(Eigen::VectorXd v)
{
    Eigen::VectorXd tmp;
    tmp = v-mean;
    double chi2 = (icov*tmp).dot(tmp);
    return chi2;
}

double Nominal::Bin::score(Eigen::VectorXd v)
{
    return chi2_cdf(chi2(v), v.size());
}

double Nominal::guessT(Eigen::VectorXd v)
{
    bool hasBest = false;
    double bestScore;
    double t = 0;

    for (auto bin : bins) {
        double score = bin.second.chi2(v);
        if (!hasBest || score < bestScore) {
            hasBest = true;
            t = bin.first/(double)nbins;
            bestScore = score;
        }
    }

    return t;
}

void Nominal::compile()
{
    for (auto &entry : bins) {
        entry.second.compile(dimensions);
    }
}

void Nominal::saveToJson(std::string filename)
{
    Json::Value json(Json::arrayValue);
    Json::StyledWriter writer;

    int k = 0;
    for (auto entry : bins) {
        auto bin = entry.second;
        Json::Value b(Json::arrayValue);
        int m = 0;
        b[0] = eigenToJson(bin.mean);
        b[1] = eigenToJson(bin.cov);
        b[2] = eigenToJson(bin.icov);
        json[k++] = b;
    }

    std::ofstream of(filename);
    of << writer.write(json);
    of.close();
}

void Nominal::loadJson(std::string filename)
{
    Json::Value json;
    Json::Reader reader;

    std::ifstream i(filename);
    std::string data((std::istreambuf_iterator<char>(i)),
                     std::istreambuf_iterator<char>());
    i.close();

    if (reader.parse(data, json)) {
        int k = 0;
        for (auto e : json) {
            bins[k].values.clear();
            bins[k].mean = jsonToEigenVector(e[0]);
            bins[k].cov = jsonToEigenMatrix(e[1]);
            bins[k].icov = jsonToEigenMatrix(e[2]);
            k++;
        }
    }
}

