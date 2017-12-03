/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
/*****************************************************************************/
/*! \file    chrono.h
 *  \author  Olivier Ly
 *  \date    2009-10
 *
 *  \brief   Basic tools for time measure.
 *****************************************************************************/
#pragma once

/*****************************************************************************/
/*!\brief A Rhoban::chrono is just a type value
 *****************************************************************************/

namespace rhoban_utils
{

#ifdef MSVC
#include <time.h>
#include <sys/timeb.h>
	struct tttt {
		long    tv_sec;
		long    tv_usec;
	};

	typedef struct tttt t_chrono;
	int gettimeofday(t_chrono *tp, void *tz = NULL);
#else
#include <time.h>
#include <sys/time.h>
	typedef struct timeval t_chrono;
#endif


	/*****************************************************************************/
	/*!\brief Reset the chrono
	 * \param chr the t_chrono to be resetted.
	 *****************************************************************************/
	void chrono_reset(t_chrono * chr);

	/*****************************************************************************/
	/*!\brief Create a new chono.
	 * \return the address of the chrono.
	 *****************************************************************************/
	t_chrono * chrono_create();

	/*****************************************************************************/
	/*!\brief kill a chrono and free its resources
	 *****************************************************************************/
	void delete_chrono(t_chrono * chr);

	/*****************************************************************************/
	/*!\brief Gives the number of second elapsed from the last timer reset.
	 *****************************************************************************/
	long int chrono_sec(const t_chrono * chr);

	/*****************************************************************************/
	/*!\brief Gives the number of milli second elapsed from the last timer reset.
	 *****************************************************************************/
	long int chrono_msec(const t_chrono * chr);

	/*****************************************************************************/
	/*!\brief Gives the number of micro second elapsed from the last timer reset.
	 *****************************************************************************/
	long int chrono_usec(const t_chrono * chr);

class Chrono
{
    public:
        Chrono();
        double getTime() const;
		long getTimeMsec() const;
		long getTimeUsec() const;
		void reset();

		Chrono& operator+=(const Chrono& chr){
			this->chr.tv_sec += chr.chr.tv_sec;
			this->chr.tv_usec += chr.chr.tv_usec;
			return *this;
		}

		Chrono& operator-=(const Chrono& chr){
			if (this->chr.tv_usec < chr.chr.tv_usec)
			{
				this->chr.tv_usec += 1000000;
				this->chr.tv_sec -= 1;
			}
			if (this->chr.tv_usec < chr.chr.tv_usec)
			{
				int d = chr.chr.tv_usec / 1000000;
				this->chr.tv_usec -= d * 1000000;
				this->chr.tv_sec += d;
			}
			this->chr.tv_sec -= chr.chr.tv_sec;
			this->chr.tv_usec -= chr.chr.tv_usec;
			return *this;
		}

		// friends defined inside class body are inline and are hidden from non-ADL lookup
		friend Chrono operator+(Chrono chr,	const Chrono& rhs){	return chr += rhs;}

		// friends defined inside class body are inline and are hidden from non-ADL lookup
		friend Chrono operator-(Chrono chr, const Chrono& rhs){ return chr -= rhs; }

protected:
        t_chrono chr;
};

}

/*****************************************************************************/
/*****************************************************************************/
