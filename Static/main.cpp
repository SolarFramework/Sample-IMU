/**
 * @copyright Copyright 1998-2018, Yost Labs Corporation
 *
 * The Yost 3-Space API is released under the Yost 3-Space Open Source License, which allows for both
 * non-commercial use and commercial use with certain restrictions.
 *
 * For Non-Commercial Use, your use of Covered Works is governed by the GNU GPL v.3, subject to the Yost 3-Space Open
 * Source Licensing Overview and Definitions.
 *
 * For Commercial Use, a Yost Commercial/Redistribution License is required, pursuant to the Yost 3-Space Open Source
 * Licensing Overview and Definitions. Commercial Use, for the purposes of this License, means the use, reproduction
 * and/or Distribution, either directly or indirectly, of the Covered Works or any portion thereof, or a Compilation,
 * Improvement, or Modification, for Pecuniary Gain. A Yost Commercial/Redistribution License may or may not require
 * payment, depending upon the intended use.
 *
 * Full details of the Yost 3-Space Open Source License can be found in LICENSE
 * License also available online at https://yostlabs.com/support/open-source-license/
 */

#include "datastructure/MathDefinitions.h"

#include <boost/log/core.hpp>
#include <boost/timer/timer.hpp>
#include <boost/chrono.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

// ADD COMPONENTS HEADERS HERE, e.g #include "SolarComponent.h"

#include "IMUYostLabsStream.h"

using namespace std;
using namespace SolAR;
using namespace SolAR::datastructure;
using namespace SolAR::api;
using namespace SolAR::MODULES::IMUYOSTLABS;

namespace xpcf = org::bcom::xpcf;

int main(int argc, char *argv[])
{

#if NDEBUG
    boost::log::core::get()->set_logging_enabled(false);
#endif

	LOG_ADD_LOG_TO_CONSOLE();
    //LOG_ADD_LOG_TO_FILE("./SolarLog.log","r");

    // component creation
    auto imu =xpcf::ComponentFactory::createInstance<IMUYostLabsStream>()->bindTo<input::devices::IIMU>();

	// declare data structures used
	Vector3f gyro;
	Vector3f accel;
	Vector3f compass;

	//ostringstream os;
	//ofstream logFile;
	//string logFileName = "log.txt";

	// detect imu
    if (imu->start() == FrameworkReturnCode::_SUCCESS)
    {
        LOG_INFO("Logging start");

		// set a loop of 5 seconds
		std::chrono::high_resolution_clock::time_point endLoop = std::chrono::high_resolution_clock::now() + std::chrono::seconds(5);

		// get measurements from imu in loop and display them
        while (std::chrono::high_resolution_clock::now() < endLoop)
        {
            if (imu->getAllSensorsData(gyro, accel, compass) != FrameworkReturnCode::_SUCCESS)
            {
				LOG_ERROR("Error while retrieving data");
				return -1;
			}

			LOG_INFO("{}, {}, {}, {}, {}, {}, {}, {}, {}", gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[2], compass[0], compass[1], compass[2]);

			//printf("Gyroscope: %f, %f, %f\n", gyro[0], gyro[1], gyro[2]);
			//printf("Accelerometer: %f, %f, %\n", accel[0], accel[1], accel[2]);
			//printf("Magnetometer: %f, %f, %f\n", compass[0], compass[1], compass[2]);

			//os << gyro[0] << "; " << gyro[1] << "; " << gyro[2] << "; " << accel[0] << "; " << accel[1] << "; " << accel[2] << "; " << compass[0] << "; " << compass[1] << "; " << compass[2] << endl;
		}

		LOG_INFO("Logging end")
    }

	//logFile.open(logFileName);
	//logFile << os.str();
    //logFile.close();

	printf("Program finished. Press enter to continue...");
	getchar();

	return 0;
}
