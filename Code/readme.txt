Readme (Interactive Search with Mixed Attributes)
=========================
This package contains all source codes as follows.
a. Algorithm SP-Tree 
	1. It works for the special case of ISM.
	2. The code is in folder Special.
b. Algorithm GE-Graph 
	1. It works for the general case of ISM.
	2. The code is in folder General.
c. Algorithm GE-Graph(Conf) 
	1. It works for the general case of ISM (for confidence study).
	2. It considers the case in which users may not answer the questions.
	3. The code is in folder General.
d. Algorithm UH-Simplex 
	1. It is an adapted existing algorithm.
	2. The code is in folder UH.
e. Algorithm UH-Random 
	1. It is an adapted existing algorithm.
	2. The code is in folder UH.
f. Algorithm ActiveRanking 
	1. It is an adapted existing algorithm.
	2. The code is in folder ActiveRanking.
g. Algorithm Adaptive 
	1. It is an adapted existing algorithm without pruning strategies.
	2. The code is in folder Adaptive.
h. Algorithm Adaptive-Prune 
	1. It is an adapted existing algorithm with pruning strategies.
	2. The code is in folder Adaptive.

Make sure there is a folder called "input/" and a folder called "output/" under the working directory.
They will be used for storing the input/output files and some intermediate results.

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
	Update the path "/usr/local/Cellar/glpk/5.0" to the path you install the GLPK package
	
b. Execution
	./run

c. Input
	The input file contains the dataset (whose format will be described in Appendix A.)
	
d. Output
	The output will be shown on the console (whose format will be described in Appendix B.)

Example
=======
Sample input (44_2d.txt) is provided. 
Try: ./run

Appendix A. Format of Input File
------------------------------------
The format of the first line is: n d_cat d_num
n - the number of tuples in the dataset, integer
d_cat - the number of categorical attributes in the dataset, integer
d_num - the number of numerical attributes in the dataset, integer
The format of the following n lines is
-----------------------------------------------------------------
<categorical attribute 1> <categorical attribute 2> ... <<categorical attribute d_cat> <numerical attribute 1> <numerical attribute 2> ... <numerical attribute d_num> 
-----------------------------------------------------------------
Each line corresponds to a tuple.
	
Appendix B. Format of Console Output
-----------------------------------------------------------------
The format of the output is
-----------------------------------------------------------------
|         Method | # of Questions |      Time Cost | Point #ID |
-----------------------------------------------------------------
-----------------------------------------------------------------
|   Ground Truth |              - |              - |    PtID-0 |
-----------------------------------------------------------------
-----------------------------------------------------------------
|        SP-Tree |      Q-count-1 |         Time-1 |    PtID-1 |
-----------------------------------------------------------------
-----------------------------------------------------------------
|       GE-Graph |      Q-count-2 |         Time-2 |    PtID-2 |
-----------------------------------------------------------------
-----------------------------------------------------------------
| GE-Graph(Conf) |      Q-count-3 |         Time-3 |    PtID-3 |
-----------------------------------------------------------------
-----------------------------------------------------------------
|     UH-Simplex |      Q-count-4 |         Time-4 |    PtID-4 |
-----------------------------------------------------------------
-----------------------------------------------------------------
|      UH-Random |      Q-count-5 |         Time-5 |    PtID-5 |
-----------------------------------------------------------------
-----------------------------------------------------------------
|  ActiveRanking |      Q-count-6 |         Time-6 |    PtID-6 |
-----------------------------------------------------------------
-----------------------------------------------------------------
|       Adaptive |      Q-count-7 |         Time-7 |    PtID-7 |
-----------------------------------------------------------------
-----------------------------------------------------------------
| Adaptive-Prune |      Q-count-8 |         Time-8 |    PtID-8 |
-----------------------------------------------------------------
PtID-0 is the tuple ID of the user's favorite tuple (ground truth).
PtID-1 is the tuple ID of the user's favorite tuple returned by algorithm SP-Tree.
PtID-2 is the tuple ID of the user's favorite tuple returned by algorithm GE-Graph.
PtID-3 is the tuple ID of the user's favorite tuple returned by algorithm GE-Graph(Conf).
PtID-4 is the tuple ID of the user's favorite tuple returned by algorithm UH-Simplex.
PtID-5 is the tuple ID of the user's favorite tuple returned by algorithm UH-Random.
PtID-6 is the tuple ID of the user's favorite tuple returned by algorithm ActiveRanking.
PtID-7 is the tuple ID of the user's favorite tuple returned by algorithm Adaptive.
PtID-8 is the tuple ID of the user's favorite tuple returned by algorithm Adaptive-Prune.
Q-count-1 is the number of questions asked by algorithm SP-Tree.
Q-count-2 is the number of questions asked by algorithm GE-Graph.
Q-count-3 is the number of questions asked by algorithm GE-Graph(Conf).
Q-count-4 is the number of questions asked by algorithm UH-Simplex.
Q-count-5 is the number of questions asked by algorithm UH-Random.
Q-count-6 is the number of questions asked by algorithm ActiveRanking.
Q-count-7 is the number of questions asked by algorithm Adaptive.
Q-count-8 is the number of questions asked by algorithm Adaptive-Prune.
Time-1 is the execution time of algorithm SP-Tree.
Time-2 is the execution time of algorithm GE-Graph.
Time-3 is the execution time of algorithm GE-Graph(Conf).
Time-4 is the execution time of algorithm UH-Simplex.
Time-5 is the execution time of algorithm UH-Random.
Time-6 is the execution time of algorithm ActiveRanking.
Time-7 is the execution time of algorithm Adaptive.
Time-8 is the execution time of algorithm Adaptive-Prune.

For example, you might see:
-----------------------------------------------------------------
|         Method | # of Questions |      Time Cost | Point #ID |
-----------------------------------------------------------------
-----------------------------------------------------------------
|   Ground Truth |              - |              - |         2 |
-----------------------------------------------------------------
-----------------------------------------------------------------
|        Special |              6 |       0.000161 |         2 |
-----------------------------------------------------------------
-----------------------------------------------------------------
|       GE-Graph |              6 |       0.003860 |         2 |
-----------------------------------------------------------------
-----------------------------------------------------------------
| GE-Graph(Conf) |              6 |       0.003669 |         2 |
-----------------------------------------------------------------
-----------------------------------------------------------------
|      UH-Random |              9 |       0.042485 |         2 |
-----------------------------------------------------------------
-----------------------------------------------------------------
|  ActiveRanking |             26 |       0.038927 |         2 |
-----------------------------------------------------------------
-----------------------------------------------------------------
|       Adaptive |             25 |       0.015247 |         2 |
-----------------------------------------------------------------
-----------------------------------------------------------------
| Adaptive-Prune |             14 |       0.035444 |         2 |
-----------------------------------------------------------------