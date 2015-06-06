/*
 * all_tests.c
 * Copyright (C) 2015 Eddie McNally <emcn@gmx.com>
 *
 * Chess is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Chess is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "seatest.h"

void board_test_fixture( void );
void piece_test_fixture( void );

void all_tests( void )
{
	board_test_fixture();
	piece_test_fixture();
	// add new test fixtures here.
}

void my_suite_setup( void )
{
	//printf("I'm done before every single test in the suite\r\n");
}

void my_suite_teardown( void )
{
	//printf("I'm done after every single test in the suite\r\n");
}

//int main( int argc, char** argv )
//{
	//return seatest_testrunner(argc, argv, all_tests, my_suite_setup, my_suite_teardown);
//}


/*************************************************************************************************************************************************************/
/*  Everything after this point are just alternative "main" functions which show different ways you can run the tests....they don't get used in this example */
/*************************************************************************************************************************************************************/

/*
Use this if you don't have any global setup/teardown...
*/
int main_no_setup_or_teardown( int argc, char** argv )
{
	return seatest_testrunner(argc, argv, all_tests, NULL, NULL);
}

int main( int argc, char** argv )
{
	return seatest_testrunner(argc, argv, all_tests, NULL, NULL);
}

/*
Use this if you don't want to use the test runner...
*/
int main_do_it_myself( int argc, char** argv )
{
	suite_setup(my_suite_setup);
	suite_teardown(my_suite_teardown);
	return run_tests(all_tests);
}

/*
Use this if you don't want to use the test runner and don't have any global setup/teardown...
*/
int main_do_it_myself_really_simply( int argc, char** argv )
{
	return run_tests(all_tests);
}
