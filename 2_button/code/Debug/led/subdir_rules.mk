################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
led/%.obj: ../led/%.cpp $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-msp430_18.12.2.LTS/bin/cl430" -vmsp --use_hw_mpy=16 --include_path="C:/ti/ccs910/ccs/ccs_base/msp430/include" --include_path="C:/Users/Hoda/Workspace/MSP430F247_Peripheral_Interface/2_button/code" --include_path="C:/ti/ccs910/ccs/tools/compiler/ti-cgt-msp430_18.12.2.LTS/include" --advice:power=all --define=__MSP430F247__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU19 --preproc_with_compile --preproc_dependency="led/$(basename $(<F)).d_raw" --obj_directory="led" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


