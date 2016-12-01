#ifndef UTIL_GAME_H
#define UTIL_GAME_H

#include <sys/time.h>
#include <dlfcn.h>
#include <iostream>

using namespace std;

#define ROBOT_COUNT 6
#define ROBOT_RADIUS 90
#define ERROR_DISTANCE 15
#define ROBOT_DISTANCES 5
#define ROBOT_DIAMETER ( 2 * ROBOT_RADIUS )
#define PI (3.14159265358979323846)

namespace Referee
{
    const char
                Halt = 'H',
                Stop = 'S',
                Ready = ' ',
                Start = 's',
                BeginFirstHalf = '1',
                BeginHalfTime = 'h',
                BeginSecondHalf = '2',
                BeginOvertimeHalf1 = 'o',
                BeginOvertimeHalf2 = 'O',

                KickOffYellow = 'k',
                PenaltyYellow = 'p',
                DirectFreeKickYellow = 'f',
                IndirectFreeKickYellow = 'i',
                TimeoutYellow = 't',
                TimeoutEndYellow = 'z',
                GoalScoredYellow = 'g',
                DecreaseGoalScoredYellow = 'd',
                YellowCardYellow = 'y',
                RedCardYellow = 'r',

                KickOffBlue = 'K',
                PenaltyBlue = 'P',
                DirectFreeKickBlue = 'F',
                IndirectFreeKickBlue = 'I',
                TimeoutBlue = 'T',
                TimeoutEndBlue = 'Z',
                GoalScoredBlue = 'G',
                DecreaseGoalScoredBlue = 'D',
                YellowCardBlue = 'Y',
                RedCardBlue = 'R',

                Cancel = 'c';
}

namespace SpaceGameSettings
{
    enum TeamColor { BLUE, YELLOW };
    enum FieldSide { LEFT, RIGHT };
}

class UtilGame
{
public:
    static double adjustAngle ( double &angle );
    static double countTime ();
    static double m_map(double x, double in_min, double in_max, double out_min, double out_max);

};
#endif
