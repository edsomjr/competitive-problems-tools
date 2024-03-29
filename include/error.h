#ifndef CP_TOOLS_ERROR_H
#define CP_TOOLS_ERROR_H

#define CP_TOOLS_TRUE  1
#define CP_TOOLS_FALSE 0

#define CP_TOOLS_OK                     0
#define CP_TOOLS_ERROR_MISSING_ARGUMENT -1
#define CP_TOOLS_ERROR_INVALID_OPTION   -2
#define CP_TOOLS_ERROR_INVALID_COMMAND  -3

#define CP_TOOLS_ERROR_INIT_INVALID_OPTION -10
#define CP_TOOLS_ERROR_INIT_COPY_FILES     -11

#define CP_TOOLS_ERROR_CLEAN_INVALID_OPTION -20
#define CP_TOOLS_ERROR_CLEAN_DELETE_FILES   -21

#define CP_TOOLS_ERROR_GENTEX_INVALID_OPTION        -30
#define CP_TOOLS_ERROR_GENTEX_LIST_DOCUMENT_CLASSES -31
#define CP_TOOLS_ERROR_GENTEX_INVALID_OUTFILE       -32

#define CP_TOOLS_ERROR_GENPDF_INVALID_OPTION   -40
#define CP_TOOLS_ERROR_GENPDF_INVALID_OUTFILE  -41
#define CP_TOOLS_ERROR_GENPDF_INVALID_LANGUAGE -42

#define CP_TOOLS_ERROR_CHECK_MISSING_IO_FILES -50

#define CP_TOOLS_ERROR_CPP_FILESYSTEM_COPY             -60
#define CP_TOOLS_ERROR_CPP_FILESYSTEM_CREATE_DIRECTORY -61
#define CP_TOOLS_ERROR_CPP_FILESYSTEM_REMOVE           -62
#define CP_TOOLS_ERROR_CPP_FILESYSTEM_EXISTS           -63
#define CP_TOOLS_ERROR_CPP_FILESYSTEM_EQUIVALENT       -64
#define CP_TOOLS_ERROR_CPP_FILESYSTEM_IS_DIRECTORY     -65
#define CP_TOOLS_ERROR_CPP_FILESYSTEM_IS_FILE          -66
#define CP_TOOLS_ERROR_CPP_FILESYSTEM_RENAME           -67

#define CP_TOOLS_ERROR_SH_BUILD_EXT_NOT_FOUND   -100
#define CP_TOOLS_ERROR_SH_CPP_COMPILATION_ERROR -101
#define CP_TOOLS_ERROR_SH_PROCESS_ERROR         -102
#define CP_TOOLS_ERROR_SH_PDFLATEX_ERROR        -103
#define CP_TOOLS_ERROR_SH_EXEC_ERROR            -104
#define CP_TOOLS_ERROR_SH_PY_BUILD_ERROR        -105
#define CP_TOOLS_ERROR_SH_POPEN_FAILED          -106
#define CP_TOOLS_ERROR_SH_FILE_NOT_FOUND        -107

#define CP_TOOLS_ERROR_CHECK_MISSING_VALIDATOR  -120
#define CP_TOOLS_ERROR_CHECK_INVALID_INPUT_FILE -121
#define CP_TOOLS_ERROR_CHECK_MISSING_TESTS      -122
#define CP_TOOLS_ERROR_CHECK_INVALID_VEREDICT   -123
#define CP_TOOLS_ERROR_CHECK_TEST_FAILED        -124
#define CP_TOOLS_ERROR_CHECK_MISSING_CHECKER    -125
#define CP_TOOLS_ERROR_CHECK_MISSING_SOLUTION   -126

#define CP_TOOLS_ERROR_JUDGE_MISSING_CHECKER    -140
#define CP_TOOLS_ERROR_JUDGE_MISSING_VALIDATOR  -141
#define CP_TOOLS_ERROR_JUDGE_MISSING_TOOL       -142
#define CP_TOOLS_ERROR_JUDGE_INVALID_INPUT_FILE -143

#define CP_TOOLS_ERROR_TASK_INVALID_TOOL -150

#define CP_TOOLS_ERROR_POLYGON_INVALID_OPTION      -160
#define CP_TOOLS_ERROR_POLYGON_MUTUAL_CHOICE_ERROR -161
#define CP_TOOLS_ERROR_POLYGON_CANT_CONNECT        -162
#define CP_TOOLS_ERROR_POLYGON_NO_PROBLEM_ID       -163
#define CP_TOOLS_ERROR_POLYGON_API                 -164

#define CP_TOOLS_EXCEPTION_INEXISTENT_FILE -200

#define CP_TOOLS_ERROR_GENBOCA_FAILURE_TO_CREATE_BUILD_DIRECTORY           -300
#define CP_TOOLS_ERROR_GENBOCA_FAILURE_TO_CREATE_DESCRIPTION_DIRECTORY     -301
#define CP_TOOLS_ERROR_GENBOCA_FAILURE_TO_CREATE_IO_DIRECTORY              -302
#define CP_TOOLS_ERROR_GENBOCA_FAILURE_TO_MODIFY_CHECKER_FILE              -303
#define CP_TOOLS_ERROR_GENBOCA_FAILURE_TO_RUN_SED_ON_CHECKER_FILE          -304
#define CP_TOOLS_ERROR_GENBOCA_FAILURE_TO_ADD_EXECUTION_PERMISSION         -305
#define CP_TOOLS_ERROR_GENBOCA_FAILURE_TO_CREATE_COPY_OF_CHECKER_FILE_FILE -306




#endif
