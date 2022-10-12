Readme (Recommending Car System)
=========================
This package contains all source codes for user study on
a. Algorithm GE-Graph. 
	1. It works for the general case of ISM. 
	2. The code is in folder General
b. Algorithm UH-Simplex 
	1. It is an adapted existing algorithm.
	2. The code is in folder UH.
c. Algorithm UH-Random 
	1. It is an adapted existing algorithm.
	2. The code is in folder UH.
d. Algorithm ActiveRanking 
	1. It is an adapted existing algorithm.
	2. The code is in folder Ranking.
e. Algorithm Adaptive 
	1. It is an adapted existing algorithm.
	2. The code is in folder Adaptive.

Make sure there are folders called "input/", "output/" and "Result/" under the working directory.
They will be used for storing the input/output files and some intermediate/final results.

Usage Step
==========
a. Compilation
	mkdir build
	cd build
	cmake ..
	make

	You will need to install the GLPK package (for solving LPs) at first.
	See GLPK webpage <http://www.gnu.org/software/glpk/glpk.html>.
	Then update the path in CMakeLists.txt
		set(INC_DIR /usr/local/Cellar/glpk/5.0/include)
		set(LINK_DIR /usr/local/Cellar/glpk/5.0/lib)
	Update "/usr/local/Cellar/glpk/5.0" to the path where you install the GLPK package
	
b. Execution
	./run

c. Input
	The used car dataset is shown in input/car1.txt (whose format will be described in Appendix A.)
	
d. Output
	1. The console output will be shown on the console (whose format will be described in Appendix B.)
	2. The user log could be found in Result/result.txt (whose format will be described in Appendix C.)

Example
=======
Try: ./run
Sample user log is provided: Result/sample.txt

Appendix A. Format of Input File
------------------------------------
For input/car1.txt (the used car dataset with four attributes: price, year, power and used KM):
	The format of the first line is: n d_cat d_num
	n - the number of cars in the dataset, integer
	d_cat - the number of categorical attributes in the dataset, integer
	d_num - the number of numerical attributes in the dataset, integer
	The format of the following n lines is
	<fuel type> <price> <year of manufacture> <horse power> <used kilometers>.
	Each line corresponds to a car
	
Appendix B. Format of Console Output
------------------------------------
The console output consists of two components: (a) interaction and (b) evaluation
The recommending car system interacts with you for different rounds.

a. Interaction
	Each round corresponds to an algorithm in our system. In each round, you will be 
	presented consecutive questions. Each question consists of two cars and asks you to 
	choose the one you favor more.
	For example, you might see:
	Please choose the car you favor more:
	-------------------------------------------------------------------------
                  Fuel    Price($)        Year     PowerPS    Used KM
	-------------------------------------------------------------------------
  	Option 1    benzin       28800        2013         432       20000
	-------------------------------------------------------------------------
  	Option 2    diesel       28600        2015         181       60000
	-------------------------------------------------------------------------
	Your choice: 
	
b. Evaluation
	You need to evaluate the algorithms in four ways. (1) Satisfaction, (2) Boredness, 
	(3) Rank on the returned cars, and (4) Rank on the algorithms 

	(1) Satisfaction
		At the end of each round, it will ask you to give a score to indicate how satisfied you
		are with the returned car. 
		For example, you might see:
		-------------------------------------------------------------------------
		    The number of questions asked:    18 
		-------------------------------------------------------------------------
		    Result      Fuel    Price($)        Year     PowerPS    Used KM
		-------------------------------------------------------------------------
		       Car    benzin       16800        2005         500      125000
		-------------------------------------------------------------------------
		Please give a number from 1 to 10 (i.e., 1, 2, .., 10) to indicate how 
		satisfied you are when seeing the recommended car. (Note: 10 denotes that 
		you are very satisfied with the recommended car and 1 denotes that you are
		unsatisfied with the recommended car):


	(2) Boredness
		At the end of each round, it will ask you to give a score to indicate how bored you
		feel when you are asked with XX questions in this round in order to obtain your 
		favorite car. 
		For example, you might see:
		-------------------------------------------------------------------------
		    The number of questions asked:    18 
		-------------------------------------------------------------------------
		    Result      Fuel    Price($)        Year     PowerPS    Used KM
		-------------------------------------------------------------------------
		       Car    benzin       16800        2005         500      125000
		-------------------------------------------------------------------------
		Please give a number from 1 to 10 (i.e., 1, 2, .., 10) to indicate how 
		bored you feel when you are asked with 18 questions in this round in 
		order to obtain your favorite car (Note: 10 denotes that you feel the 
		most bored and 1 denotes that you feel the least bored.):



	(3) Rank on the returned cars
		After all the rounds, it will ask you to compare the recommended cars returned by 
		different rounds (i.e., different algorithms). You will be presented all the 
		recommended cars and you need to give an order of the recommended cars based on 
		your preference. 
		For example, you might see:

		The recommended tuples: 
		-------------------------------------------------------------------------
		       Car      Fuel    Price($)        Year     PowerPS    Used KM
		-------------------------------------------------------------------------
		         1    benzin       28800        2013         432       20000
		-------------------------------------------------------------------------
		         2    benzin       16800        2005         500      125000
		-------------------------------------------------------------------------
		Please give an order of preference on the shown used car numbers (e.g., 1 2), 
		where the first one is the most preferred car and the last one is the 
		least preferred car: 


		(4) Rank on the algorithms 
		After all the rounds, it will ask you to compare all the rounds (i.e., algorithms). 
		You will be presented all the rounds (each with the number of questions asked and 
		the recommended car). You need to give an order of the rounds based on the numbers 
		of questions asked and the recommended cars.
		For example, you might see:
		
		Method: 1
		-------------------------------------------------------------------------
		    The number of questions asked:    41 
		-------------------------------------------------------------------------
		    Result      Fuel    Price($)        Year     PowerPS    Used KM
		-------------------------------------------------------------------------
		       Car    benzin       28800        2013         432       20000
		-------------------------------------------------------------------------

		Method: 2
		-------------------------------------------------------------------------
		    The number of questions asked:    18 
		-------------------------------------------------------------------------
		    Result      Fuel    Price($)        Year     PowerPS    Used KM
		-------------------------------------------------------------------------
		       Car    benzin       16800        2005         500      125000
		-------------------------------------------------------------------------

		Method: 3
		-------------------------------------------------------------------------
 		   The number of questions asked:    14 
		-------------------------------------------------------------------------
		    Result      Fuel    Price($)        Year     PowerPS    Used KM
		-------------------------------------------------------------------------
		       Car    benzin       16800        2005         500      125000
		-------------------------------------------------------------------------

		Method: 4
		-------------------------------------------------------------------------
		    The number of questions asked:    60 
		-------------------------------------------------------------------------
		    Result      Fuel    Price($)        Year     PowerPS    Used KM
		-------------------------------------------------------------------------
		       Car    benzin       16800        2005         500      125000
		-------------------------------------------------------------------------

		Method: 5
		-------------------------------------------------------------------------
		    The number of questions asked:    15 
		-------------------------------------------------------------------------
		    Result      Fuel    Price($)        Year     PowerPS    Used KM
		-------------------------------------------------------------------------
 		      Car    benzin       16800        2005         500      125000
		-------------------------------------------------------------------------

		Please give an order of preference on the method numbers (e.g., 1 2 3 4 5) 
		by considering the number of questions asked and the returned car in each 
		method, where the first one is the most preferred method and the last one 
		is the least preferred method:

	
Appendix C. Format of User Log
------------------------------------
	The user log file is shown in Result/result.txt.
	It contains two parts: (a) The result of rounds and (b) The result of order

a. The result of rounds
	Line 1 shows the algorithm name and the number of questions asked.
	Lines 2-6 show the recommended car.
	Line 7 shows the score of satisfaction
	Line 8 shows the score of boredness
	For example, you might see:

	Algorithm: UH-Random   Question: 15
	----------------------------------------------------------------------------
	    Result      Fuel    Price($)        Year     PowerPS    Used KM
	----------------------------------------------------------------------------
	       Car    benzin       16800        2005         500      125000
	----------------------------------------------------------------------------
	Satisfaction: 8
	Boredness: 4
	
b. The result of order. It contains (1) the order of recommended cars and (2) the 
   order of algorithms.

	(1) The order of recommended cars
	Line 1 is the title "Returned car order".
	The rest lines show the order of the recommended cars (which are returned by 
	different rounds) based on the user preference, where the first one is the most 
	preferred tuple and the last one is the least preferred tuple.
	For example, you might see:

	Returned car order: 
	-------------------------------------------------------------------------
			1    benzin       16800        2005         500      125000
	-------------------------------------------------------------------------
			2    benzin       28800        2013         432       20000
	-------------------------------------------------------------------------


	(2) The order of algorithms
	Line 1 is the title "Algorithm order".
	The rest lines show the order of the algorithms based on the user preference, 
	where the first one is the most preferred algorithm and the last one is the 
	least preferred algorithm.
	For example, you might see:

	Algorithm order: 
	-------------------------------------------------------------------------
	Case:3           The number of questions asked: 14
	-------------------------------------------------------------------------
		Car    benzin       16800        2005         500      125000
	-------------------------------------------------------------------------
	-------------------------------------------------------------------------
	Case:5           The number of questions asked: 15
	-------------------------------------------------------------------------
		Car    benzin       16800        2005         500      125000
	-------------------------------------------------------------------------
	-------------------------------------------------------------------------
	Case:2           The number of questions asked: 18
	-------------------------------------------------------------------------
		Car    benzin       16800        2005         500      125000
	-------------------------------------------------------------------------
	-------------------------------------------------------------------------
	Case:1           The number of questions asked: 41
	-------------------------------------------------------------------------
		Car    benzin       28800        2013         432       20000
	-------------------------------------------------------------------------
	-------------------------------------------------------------------------
	Case:4           The number of questions asked: 60
	-------------------------------------------------------------------------
		Car    benzin       16800        2005         500      125000
	-------------------------------------------------------------------------








