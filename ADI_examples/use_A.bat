ECHO OFF
CLS
ECHO ****************************************************************************************************
ECHO * This batch file use_A.bat copies the following files           					*
ECHO * - abcc_network_data_parameters.c				      					*
ECHO * - abcc_driver_config.h				      	      					*
ECHO * for the speed example application to the example_application folder.				*
ECHO *													*
ECHO * Before copying these files the existing files are copied to the folder Backup_Last_ADI_example.	*
ECHO *													*
ECHO * It also copies an info file about this copying process.        					*
ECHO *													*
ECHO * And it copies the corresponding network configuration files for          			*
ECHO * - EtherCAT											*
ECHO * - EtherNet/IP											*
ECHO * - PROFIBUS											*
ECHO * - PROFINET											*
ECHO * to the folder example_application\cfg-files after having deleted the former cfg-files!!!		*
ECHO *													*
ECHO ****************************************************************************************************  
pause

REM goto the root directory
cd..	& REM level root

ECHO Backing up the last ADI example
md ADI_examples\Backup_Last_ADI_example
copy src\example_application\abcc_network_data_parameters.c ADI_examples\Backup_Last_ADI_example\*.*
copy src\abcc_adaptation\abcc_driver_config.h ADI_examples\Backup_Last_ADI_example\*.*
REM pause 

ECHO Backing up the last xxx_selected.txt file
del ADI_examples\Backup_Last_ADI_example\*.txt
copy ADI_examples\*.txt ADI_examples\Backup_Last_ADI_example\*.*
del ADI_examples\*.txt
REM pause

ECHO Copying the file abcc_network_data_parameters.c 
copy ADI_examples\A_speed_example\abcc_network_data_parameters.c src\example_application\*.*
REM pause 

ECHO Copying the file abcc_driver_config.h
copy ADI_examples\A_speed_example\abcc_driver_config.h src\abcc_adaptation\*.*
REM pause

ECHO Deleting the cfg files of the other ADI examples
rd /S /Q cfg_files
REM pause

ECHO Copy cfg files
xcopy /s ADI_examples\A_speed_example\cfg_files cfg_files\*.*
REM pause

ECHO Copying the file A_speed_example_selected.txt
copy ADI_examples\A_speed_example\A_speed_example_selected.txt  ADI_examples\*.*
REM pause

ECHO ****************************************************************************************************
ECHO * Batch file executed succcessfully!								*
ECHO ****************************************************************************************************
pause
 