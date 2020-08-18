# How to get the module list of a trigger and check the last active one 

This example shows a simple way to obtain the last active module (filter) of a trigger.  For this, we first choose a specific trigger (HLT_Jet190_v6, in this specific case), show how to dump all the modules in that trigger (this is done at the begining of each run through the HLTConfigProvider) and then print out the label of the last active module for each event.  If for the latter action one wants to consider all the triggers, the "@" character must be set in the configuration file.

## Usage Instruction

Please follow the general instruccions in the main *README* file of this repository.

