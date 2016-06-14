#include "magellan/magellan.hpp"
#include <ccinfra/algo/loop.h>
#include "ccinfra/base/Keywords.h"
#include "ccinfra/gof/Singleton.h"
#include "ccinfra/base/NullPtr.h"
#include "ccinfra/gof/State.h"

USING_HAMCREST_NS

// Assume exit a singing robot:
// It has 3 states:          { closed,  opened,  wait_charged }
// It can receive 4 events : { close, open,  play,  charge }
// It can do 5 actions :     { mute,  bibi,  sing,  chargeEnergy,  consumeEnergy }
// It has a condition :      { isExhausted }
// It can sing twice after one charge, and can always bibi when needed.

// The following is the details of the robot acting!

/************************************************************************************************************/
/*   Source State | Pre-Condition  |   Event    |   Action            |  Post-Condition | Destination State */
/*----------------|----------------|------------|---------------------|-----------------|-------------------*/
/*   closed       |     none       |   close    |   mute              |  none           |  closed           */
/*----------------|----------------|------------|---------------------|-----------------|-------------------*/
/*   closed       |   isExhausted  |   open     |   bibi              |  none           |  closed           */
/*----------------|----------------|------------|---------------------|-----------------|-------------------*/
/*   closed       |   !isExhausted |   open     |   none              |  none           |  opened           */
/*----------------|----------------|------------|---------------------|-----------------|-------------------*/
/*   closed       |     none       |   play     |   none              |  none           |  closed           */
/*----------------|----------------|------------|---------------------|-----------------|-------------------*/
/*   closed       |     none       |   charge   |   chargeEnergy      |  none           |  closed           */
/*----------------|----------------|------------|---------------------|-----------------|-------------------*/
/*   opened       |     none       |   close    |   mute              |  none           |  closed           */
/*----------------|----------------|------------|---------------------|-----------------|-------------------*/
/*   opened       |     none       |   open     |   none              |  none           |  opened           */
/*----------------|----------------|------------|---------------------|-----------------|-------------------*/
/*   opened       |   !isExhausted |   play     |sing & consumeEnergy |  !isExhausted   |  opened           */
/*----------------|----------------|------------|---------------------|-----------------|-------------------*/
/*   opened       |   !isExhausted |   play     |sing & consumeEnergy |  isExhausted    |  wait_charged     */
/*----------------|----------------|------------|---------------------|-----------------|-------------------*/
/*   opened       |     none       |   charge   |   chargeEnergy      |  none           |  opened           */
/*----------------|----------------|------------|---------------------|-----------------|-------------------*/
/*   wait_charged |     none       |   close    |   mute              |  none           |  closed           */
/*----------------|----------------|------------|---------------------|-----------------|-------------------*/
/*   wait_charged |     none       |   open     |   bibi              |  none           |  wait_charged     */
/*----------------|----------------|------------|---------------------|-----------------|-------------------*/
/*   wait_charged |     none       |   play     |   bibi              |  none           |  wait_charged     */
/*----------------|----------------|------------|---------------------|-----------------|-------------------*/
/*   wait_charged |     none       |   charge   |   chargeEnergy      |  none           |  opened           */
/************************************************************************************************************/

// Below is the implementation and tests of the singing robot!

namespace
{
    DEF_INTERFACE(Audio)
    {
        ABSTRACT(void bibi());
        ABSTRACT(void sing());
        ABSTRACT(void mute());
    };

//    struct Robot
//    {
//        enum State
//        {
//            closed,
//            opened,
//            wait_charged
//        };
//
//        enum
//        {
//            max_sing_times_in_one_charge = 2
//        };
//
//        Robot(Audio& audio)
//        : audio(audio)
//        , state(closed)
//        , leftSingTimes(0)
//        {
//        }
//
//        void open()
//        {
//            if(state == closed)
//            {
//                if(!isExhausted())
//                {
//                    state = opened;
//                }
//                else
//                {
//                    bibi();
//                }
//            }
//            else if(state == wait_charged)
//            {
//                audio.bibi();
//            }
//        }
//
//        void play()
//        {
//            if(state == opened)
//            {
//                if(!isExhausted())
//                {
//                    sing();
//                    consumeEnergy();
//                    if(isExhausted())
//                    {
//                        state = wait_charged;
//                    }
//                }
//            }
//            else if(state == wait_charged)
//            {
//                bibi();
//            }
//        }
//
//        void charge()
//        {
//            chargeEnergy();
//
//            if(state == wait_charged)
//            {
//                state = opened;
//            }
//        }
//
//        void close()
//        {
//            mute();
//            state = closed;
//        }
//
//        State getCurrentState() const
//        {
//            return state;
//        }
//
//        bool isExhausted() const
//        {
//            return leftSingTimes == 0;
//        }
//
//    private:
//        void mute()
//        {
//            audio.mute();
//        }
//
//        void bibi()
//        {
//            audio.bibi();
//        }
//
//        void sing()
//        {
//            audio.sing();
//        }
//
//        void consumeEnergy()
//        {
//            leftSingTimes--;
//        }
//
//        void chargeEnergy()
//        {
//            leftSingTimes = max_sing_times_in_one_charge;
//        }
//
//    private:
//        Audio& audio;
//        State state;
//        U8 leftSingTimes;
//    };

    struct Robot
    {
        enum State
        {
            closed,
            opened,
            wait_charged
        };

        enum
        {
            max_sing_times_in_one_charge = 2
        };

        Robot(Audio& audio)
        : audio(audio)
        , state(__null_ptr)
        , leftSingTimes(0)
        {
            __GOTO_STATE(Closed);
        }

        void open();
        void play();
        void charge();
        void close();
        State getCurrentState() const;

        bool isExhausted() const
        {
            return leftSingTimes == 0;
        }

    private:
        void mute()
        {
            audio.mute();
        }

        void bibi()
        {
            audio.bibi();
        }

        void sing()
        {
            audio.sing();
        }

        void consumeEnergy()
        {
            leftSingTimes--;
        }

        void chargeEnergy()
        {
            leftSingTimes = max_sing_times_in_one_charge;
        }

    private:
        __USING_GOF_STATE;
        __HAS_STATE(Closed);
        __HAS_STATE(Opened);
        __HAS_STATE(WaitCharged);

    private:
        Audio& audio;
        U8 leftSingTimes;
    };

    __DEF_GOF_STATE_INTERFACE(Robot)
    {
        ABSTRACT(State getState() const);
        DEFAULT(void, open(Robot&));
        DEFAULT(void, play(Robot&));
        DEFAULT(void, charge(Robot&));
        virtual void close(Robot& THIS)
        {
            THIS.mute();
            THIS.__GOTO_STATE(Closed);
        }
    };

    __DEF_GOF_STATE(Robot, Closed)
    {
    private:
        OVERRIDE(void open(Robot& THIS))
        {
            if(THIS.isExhausted())
            {
                THIS.bibi();
            }
            else
            {
                THIS.__GOTO_STATE(Opened);
            }
        }

        OVERRIDE(void charge(Robot& THIS))
        {
            THIS.chargeEnergy();
        }

        OVERRIDE(State getState() const)
        {
            return closed;
        }
    };

    __DEF_GOF_STATE(Robot, Opened)
    {
    private:
        OVERRIDE(void play(Robot& THIS))
        {
            if(THIS.isExhausted()) return;

            THIS.sing();
            THIS.consumeEnergy();

            if(THIS.isExhausted())
            {
                THIS.__GOTO_STATE(WaitCharged);
            }
        }

        OVERRIDE(void charge(Robot& THIS))
        {
            THIS.chargeEnergy();
        }

        OVERRIDE(State getState() const)
        {
            return opened;
        }
    };

    __DEF_GOF_STATE(Robot, WaitCharged)
    {
    private:
        OVERRIDE(void open(Robot& THIS))
        {
            THIS.bibi();
        }

        OVERRIDE(void play(Robot& THIS))
        {
            THIS.bibi();
        }

        OVERRIDE(void charge(Robot& THIS))
        {
            THIS.chargeEnergy();
            THIS.__GOTO_STATE(Opened);
        }

        OVERRIDE(State getState() const)
        {
            return wait_charged;
        }
    };

    __REGISTER_STATE(Robot, Closed);
    __REGISTER_STATE(Robot, Opened);
    __REGISTER_STATE(Robot, WaitCharged);

    void Robot::open()
    {
        state->open(*this);
    }

    void Robot::play()
    {
        state->play(*this);
    }

    void Robot::charge()
    {
        state->charge(*this);
    }

    void Robot::close()
    {
        state->close(*this);
    }

    Robot::State Robot::getCurrentState() const
    {
        return state->getState();
    }

}

FIXTURE(GofStateTest)
{
    GofStateTest() : robot(audio)
    {
    }

    void gotoExhaustedInWaitChargedState()
    {
        robot.charge();
        robot.open();

        FOR_EACH_0(i, Robot::max_sing_times_in_one_charge)
        {
            robot.play();
        }
    }

    U8 getSingTimes() const
    {
        return audio.singTimes;
    }

    U8 getBibiTimes() const
    {
        return audio.bibiTimes;
    }

    bool isMuteNow() const
    {
        return audio.isInMute;
    }

protected:
    struct MockAudio : Audio
    {
        MockAudio()
        : isInMute(true)
        , singTimes(0)
        , bibiTimes(0)
        {
        }

        bool isInMute;
        U8 singTimes;
        U8 bibiTimes;

    private:
        OVERRIDE(void mute())
        {
            isInMute = true;
        }

        OVERRIDE(void sing())
        {
            isInMute = false;
            singTimes++;
        }
        OVERRIDE(void bibi())
        {
            isInMute = false;
            bibiTimes++;
        }
    };


    MockAudio audio;
    Robot robot;


	TEST("should_init_at_closed_state_mute_action_without_energy")
	{
		ASSERT_THAT(robot.getCurrentState(), eq(Robot::closed));
		ASSERT_THAT(robot.isExhausted(), be_true());
		ASSERT_THAT(isMuteNow(), be_true());
	}

	TEST("should_closed_and_mute_when_send_close_in_closed_state")
	{
		robot.close();

		ASSERT_THAT(robot.getCurrentState(), eq(Robot::closed));
		ASSERT_THAT(robot.isExhausted(), be_true());
		ASSERT_THAT(isMuteNow(), be_true());
	}

	TEST("should_closed_and_bibi_when_send_open_in_closed_state_without_energy")
	{
		robot.open();

		ASSERT_THAT(robot.getCurrentState(), eq(Robot::closed));
		ASSERT_THAT(robot.isExhausted(), be_true());
		ASSERT_THAT(getBibiTimes(), eq(1));
		ASSERT_THAT(isMuteNow(), be_false());
	}

	TEST("should_closed_and_mute_when_send_play_in_closed_state_without_energy")
	{
		robot.play();

		ASSERT_THAT(robot.getCurrentState(), eq(Robot::closed));
		ASSERT_THAT(robot.isExhausted(), be_true());
		ASSERT_THAT(isMuteNow(), be_true());
	}

	TEST("should_closed_and_mute_but_has_energy_when_send_charge_in_closed_state_without_energy")
	{
		robot.charge();

		ASSERT_THAT(robot.getCurrentState(), eq(Robot::closed));
		ASSERT_THAT(robot.isExhausted(), be_false());
		ASSERT_THAT(isMuteNow(), be_true());
	}

	TEST("should_closed_and_mute_when_send_play_in_closed_state_with_energy")
	{
		robot.charge();
		robot.play();

		ASSERT_THAT(robot.getCurrentState(), eq(Robot::closed));
		ASSERT_THAT(robot.isExhausted(), be_false());
		ASSERT_THAT(isMuteNow(), be_true());
	}

	TEST("should_open_and_mute_when_send_open_in_closed_state_with_energy")
	{
		robot.charge();
		robot.open();

		ASSERT_THAT(robot.getCurrentState(), eq(Robot::opened));
		ASSERT_THAT(robot.isExhausted(), be_false());
		ASSERT_THAT(isMuteNow(), be_true());
	}

	TEST("should_open_and_sing_when_send_play_in_open_state_with_energy")
	{
		robot.charge();
		robot.open();
		robot.play();

		ASSERT_THAT(robot.getCurrentState(), eq(Robot::opened));
		ASSERT_THAT(robot.isExhausted(), be_false());
		ASSERT_THAT(getSingTimes(), eq(1));
		ASSERT_THAT(isMuteNow(), be_false());
	}

	TEST("should_closed_and_mute_when_send_close_after_sing")
	{
		robot.charge();
		robot.open();
		robot.play();
		robot.close();

		ASSERT_THAT(robot.getCurrentState(), eq(Robot::closed));
		ASSERT_THAT(robot.isExhausted(), be_false());
		ASSERT_THAT(getSingTimes(), eq(1));
		ASSERT_THAT(isMuteNow(), be_true());
	}

	TEST("should_closed_and_mute_when_send_close_in_open_state_with_energy")
	{
		robot.charge();
		robot.open();
		robot.close();

		ASSERT_THAT(robot.getCurrentState(), eq(Robot::closed));
		ASSERT_THAT(robot.isExhausted(), be_false());
		ASSERT_THAT(isMuteNow(), be_true());
	}

	TEST("should_continue_sing_when_send_open_close_open_play_with_energy")
	{
		robot.charge();
		robot.open();
		robot.close();
		robot.open();
		robot.play();

		ASSERT_THAT(robot.getCurrentState(), eq(Robot::opened));
		ASSERT_THAT(robot.isExhausted(), be_false());
		ASSERT_THAT(getSingTimes(), eq(1));
	}

	TEST("should_goto_wait_charged_state_when_play_twice_in_open_state_with_energy")
	{
		robot.charge();
		robot.open();

		robot.play();
		robot.play();

		ASSERT_THAT(robot.getCurrentState(), eq(Robot::wait_charged));
		ASSERT_THAT(robot.isExhausted(), be_true());
		ASSERT_THAT(getSingTimes(), eq(2));
	}

	TEST("should_continue_play_when_charge_in_open_state")
	{
		robot.charge();
		robot.open();
		robot.play();
		robot.charge();
		robot.play();

		ASSERT_THAT(robot.getCurrentState(), eq(Robot::opened));
		ASSERT_THAT(robot.isExhausted(), be_false());
		ASSERT_THAT(getSingTimes(), eq(2));
	}

	TEST("should_bibi_when_send_play_in_wait_charged_state")
	{
		gotoExhaustedInWaitChargedState();
		robot.play();

		ASSERT_THAT(robot.getCurrentState(), eq(Robot::wait_charged));
		ASSERT_THAT(robot.isExhausted(), be_true());
		ASSERT_THAT(getSingTimes(), eq(2));
		ASSERT_THAT(getBibiTimes(), eq(1));
	}

	TEST("should_bibi_when_send_open_in_wait_charged_state")
	{
		gotoExhaustedInWaitChargedState();
		robot.open();

		ASSERT_THAT(robot.getCurrentState(), eq(Robot::wait_charged));
		ASSERT_THAT(robot.isExhausted(), be_true());
		ASSERT_THAT(getSingTimes(), eq(2));
		ASSERT_THAT(getBibiTimes(), eq(1));
		ASSERT_THAT(isMuteNow(), be_false());
	}

	TEST("should_closed_and_mute_without_energy_when_send_close_in_wait_charged_state")
	{
		gotoExhaustedInWaitChargedState();
		robot.close();

		ASSERT_THAT(robot.getCurrentState(), eq(Robot::closed));
		ASSERT_THAT(robot.isExhausted(), be_true());
		ASSERT_THAT(getSingTimes(), eq(2));
		ASSERT_THAT(getBibiTimes(), eq(0));
		ASSERT_THAT(isMuteNow(), be_true());
	}

	TEST("should_goto_opened_when_charge_in_wait_charged_state")
	{
		gotoExhaustedInWaitChargedState();
		robot.charge();

		ASSERT_THAT(robot.getCurrentState(), eq(Robot::opened));
		ASSERT_THAT(robot.isExhausted(), be_false());
	}

	TEST("should_continue_sing_when_opened_again_by_charging")
	{
		gotoExhaustedInWaitChargedState();
		robot.charge();
		robot.play();

		ASSERT_THAT(robot.getCurrentState(), eq(Robot::opened));
		ASSERT_THAT(robot.isExhausted(), be_false());
		ASSERT_THAT(getSingTimes(), eq(3));
		ASSERT_THAT(isMuteNow(), be_false());
	}
};
