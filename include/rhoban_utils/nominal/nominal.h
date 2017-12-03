#pragma once

#include <map>
#include <vector>
#include <string>
#include <Eigen/Dense>
#include <json/json.h>

namespace rhoban_utils
{

class Nominal
{
    public:
        struct Bin
        {
            std::vector<Eigen::VectorXd> values;

            void add(Eigen::VectorXd v);
            void compile(int dimensions);
            double chi2(Eigen::VectorXd v);
            double score(Eigen::VectorXd v);

            Eigen::VectorXd mean;
            Eigen::MatrixXd cov;
            Eigen::MatrixXd icov;
        };

        Nominal(int dimensions, int bins = 100);
        Bin &getBin(double t);
        double guessT(Eigen::VectorXd v);
        void add(double t, Eigen::VectorXd v);
        void compile();
        void saveToJson(std::string filename);
        void loadJson(std::string filename);

    protected:
        int dimensions;
        int nbins;
        std::map<int, Bin> bins;
};

}
