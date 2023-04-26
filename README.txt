# Soda-CanSat-V2.4-
README

[Project Description]:
Small-scale educational CanSat that fits in a normal soda can. 

[Repository Overview]:
Code provided here is to be uploaded to the indivudal CanSats and to the Base Station

[Relevant files in this repository include]:
A100_SodaCanSat-DHT_V1
A200_SodaCanSat-DHT_V2
MorseBaseStation_V1
CanSat_BaseStation_Model4


[Use of these files is as follows]:

A100_SodaCanSat-DHT_V1
-- Flash to Arduino in first CanSat
A200_SodaCanSat-DHT_V2
-- Flash to Arduino in second CanSat
MorseBaseStation_V1
--Flash to Arduino in SCR / Query Radio
CanSat_BaseStation_Model4
--Open Simulink model on Base Station computer

[Device wiring for all CanSats]
//FS100A ==> Arduino
//GND --> GND
//Data --> D12
//VCC --> 5V

//XLC-RF-5V ==> Arduino
//GND --> GND
//Data --> D11
//VCC --> 5V

//DHT-22 ==> Arduino
//(-) --> GND
//(+) --> 5v
// OUT --> D8

[Device wiring for SCR / Query Radio]:
//FS100A ==> Arduino
//GND --> GND
//Data --> D12
//VCC --> 5V

//XLC-RF-5V ==> Arduino
//GND --> GND
//Data --> D11
//VCC --> 5V
