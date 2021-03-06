EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "BMT"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:+BATT #PWR?
U 1 1 5F23D981
P 1100 1600
F 0 "#PWR?" H 1100 1450 50  0001 C CNN
F 1 "+BATT" H 1115 1773 50  0000 C CNN
F 2 "" H 1100 1600 50  0001 C CNN
F 3 "" H 1100 1600 50  0001 C CNN
	1    1100 1600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F23D9C6
P 1050 6900
F 0 "#PWR?" H 1050 6650 50  0001 C CNN
F 1 "GND" H 1055 6727 50  0000 C CNN
F 2 "" H 1050 6900 50  0001 C CNN
F 3 "" H 1050 6900 50  0001 C CNN
	1    1050 6900
	1    0    0    -1  
$EndComp
$Comp
L Connector:LEMO5 J?
U 1 1 5F23DDCC
P 2250 2750
F 0 "J?" H 2250 3167 50  0000 C CNN
F 1 "Kunteng LCDx Display Connector" H 2250 3076 50  0000 C CNN
F 2 "" H 2250 2750 50  0001 C CNN
F 3 " ~" H 2250 2750 50  0001 C CNN
	1    2250 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 4350 1950 2850
Wire Wire Line
	1950 1600 1950 2550
$Comp
L Device:R R?
U 1 1 5F23DFBE
P 2350 3550
F 0 "R?" H 2420 3596 50  0000 L CNN
F 1 "1k" H 2420 3505 50  0000 L CNN
F 2 "" V 2280 3550 50  0001 C CNN
F 3 "~" H 2350 3550 50  0001 C CNN
	1    2350 3550
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5F23E044
P 1250 3900
F 0 "R?" H 1320 3946 50  0000 L CNN
F 1 "22k" H 1320 3855 50  0000 L CNN
F 2 "" V 1180 3900 50  0001 C CNN
F 3 "~" H 1250 3900 50  0001 C CNN
	1    1250 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 2550 2850 2550
Wire Wire Line
	1250 4050 1250 4100
Wire Wire Line
	1400 3550 1250 3550
Connection ~ 1250 3550
Wire Wire Line
	1250 3550 1250 3750
Wire Wire Line
	850  4100 1250 4100
$Comp
L Connector:Conn_01x04_Male J?
U 1 1 5F240083
P 8100 1500
F 0 "J?" H 8072 1473 50  0000 R CNN
F 1 "Rosenberg Motor" H 8072 1382 50  0000 R CNN
F 2 "" H 8100 1500 50  0001 C CNN
F 3 "~" H 8100 1500 50  0001 C CNN
	1    8100 1500
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female J?
U 1 1 5F24043A
P 8100 950
F 0 "J?" H 8128 926 50  0000 L CNN
F 1 "SAE Power Connector Motor" H 8128 835 50  0000 L CNN
F 2 "" H 8100 950 50  0001 C CNN
F 3 "~" H 8100 950 50  0001 C CNN
	1    8100 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 1600 6900 950 
Wire Wire Line
	6900 950  7900 950 
Wire Wire Line
	7100 1500 7900 1500
Wire Wire Line
	7350 1400 7900 1400
$Comp
L Connector:Conn_01x03_Female J?
U 1 1 5F243217
P 8100 2700
F 0 "J?" H 8128 2726 50  0000 L CNN
F 1 "Brake connector" H 8128 2635 50  0000 L CNN
F 2 "" H 8100 2700 50  0001 C CNN
F 3 "~" H 8100 2700 50  0001 C CNN
	1    8100 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 2750 7550 2600
Wire Wire Line
	7550 2600 7900 2600
Wire Wire Line
	7100 2700 7900 2700
Connection ~ 7100 2700
$Comp
L bluepill:BP U?
U 1 1 5F24837F
P 4700 6200
F 0 "U?" H 4700 7450 60  0000 C CNN
F 1 "BP" H 4700 7344 60  0000 C CNN
F 2 "" H 4600 6950 60  0001 C CNN
F 3 "" H 4600 6950 60  0001 C CNN
	1    4700 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 7550 4850 7550
Wire Wire Line
	4850 7550 4850 7250
$Comp
L Interface_CAN_LIN:MCP2551-I-P U?
U 1 1 5F254E8C
P 6700 5650
F 0 "U?" H 6700 6228 50  0000 C CNN
F 1 "MCP2551-CAN Transceiver" H 6700 6137 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm" H 6700 5150 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/devicedoc/21667d.pdf" H 6700 5650 50  0001 C CNN
	1    6700 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 2750 5700 3500
Wire Wire Line
	5700 5150 4850 5150
Wire Wire Line
	5700 2750 6700 2750
Wire Wire Line
	2550 2850 3550 2850
Wire Wire Line
	3550 2850 3550 4600
Wire Wire Line
	3550 4600 5850 4600
Wire Wire Line
	5850 4600 5850 6500
Wire Wire Line
	5850 6500 5450 6500
Wire Wire Line
	2250 3150 3500 3150
Wire Wire Line
	3500 3150 3500 4700
Wire Wire Line
	3500 4700 5800 4700
Wire Wire Line
	5800 4700 5800 6400
Wire Wire Line
	5800 6400 5450 6400
Wire Wire Line
	5450 6200 5950 6200
Wire Wire Line
	5950 6200 5950 5450
Wire Wire Line
	5950 5450 6200 5450
Wire Wire Line
	5450 6300 6050 6300
Wire Wire Line
	6050 6300 6050 5550
Wire Wire Line
	6050 5550 6200 5550
Wire Wire Line
	6700 2750 6700 5250
Connection ~ 6700 2750
Wire Wire Line
	6700 2750 7300 2750
Wire Wire Line
	4850 7550 6700 7550
Wire Wire Line
	6700 7550 6700 6050
Connection ~ 4850 7550
Wire Wire Line
	7200 5550 7400 5550
Wire Wire Line
	7400 5550 7400 1600
Wire Wire Line
	7400 1600 7900 1600
Wire Wire Line
	7200 5750 7450 5750
Wire Wire Line
	7450 5750 7450 1700
Wire Wire Line
	7450 1700 7900 1700
Wire Wire Line
	7900 2800 7550 2800
Wire Wire Line
	7550 2800 7550 4900
Wire Wire Line
	7550 4900 3850 4900
Wire Wire Line
	3850 4900 3850 6600
Wire Wire Line
	3850 6600 3950 6600
Connection ~ 1250 4100
Wire Wire Line
	1950 1600 3050 1600
Connection ~ 1950 1600
Wire Wire Line
	3050 1600 3050 1750
Connection ~ 3050 1600
Wire Wire Line
	3050 1600 6900 1600
Wire Wire Line
	1250 4100 1250 4350
Wire Wire Line
	1100 1600 1950 1600
Connection ~ 1950 4350
Wire Wire Line
	7100 1050 7100 1500
Connection ~ 7100 1500
$Comp
L Device:R R?
U 1 1 5F28C11A
P 9400 5750
F 0 "R?" H 9470 5796 50  0000 L CNN
F 1 "120" H 9470 5705 50  0000 L CNN
F 2 "" V 9330 5750 50  0001 C CNN
F 3 "~" H 9400 5750 50  0001 C CNN
	1    9400 5750
	1    0    0    -1  
$EndComp
Connection ~ 7400 5550
Wire Wire Line
	7450 5800 7450 5750
Connection ~ 7450 5750
$Comp
L Connector:Conn_01x02_Female J?
U 1 1 5F29F99C
P 8700 5700
F 0 "J?" H 8727 5676 50  0000 L CNN
F 1 "CAN Terminator" H 8300 5850 50  0000 L CNN
F 2 "" H 8700 5700 50  0001 C CNN
F 3 "~" H 8700 5700 50  0001 C CNN
	1    8700 5700
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8850 5700 8900 5700
Wire Wire Line
	9100 5700 9100 5600
Wire Wire Line
	9100 5600 9400 5600
Wire Wire Line
	8850 5800 8900 5800
Wire Wire Line
	9250 5800 9250 5900
Wire Wire Line
	9250 5900 9400 5900
$Comp
L Connector:Conn_01x02_Male J?
U 1 1 5F2B86D5
P 8350 5700
F 0 "J?" H 8322 5674 50  0000 R CNN
F 1 "CAN Connector" H 8650 5500 50  0000 R CNN
F 2 "" H 8350 5700 50  0001 C CNN
F 3 "~" H 8350 5700 50  0001 C CNN
	1    8350 5700
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8150 5550 8150 5700
Wire Wire Line
	7450 5800 8150 5800
Wire Wire Line
	7400 5550 8150 5550
Connection ~ 8900 5700
Wire Wire Line
	8900 5700 9100 5700
Connection ~ 8900 5800
Wire Wire Line
	8900 5800 9250 5800
$Comp
L Connector:Conn_01x03_Female J?
U 1 1 5F2F307C
P 8150 3600
F 0 "J?" H 8178 3626 50  0000 L CNN
F 1 "Throttle connector" H 8178 3535 50  0000 L CNN
F 2 "" H 8150 3600 50  0001 C CNN
F 3 "~" H 8150 3600 50  0001 C CNN
	1    8150 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 3700 7950 3700
Wire Wire Line
	7300 2750 7300 3500
Wire Wire Line
	7300 3500 7950 3500
Connection ~ 7300 2750
Wire Wire Line
	7300 2750 7550 2750
Wire Wire Line
	7950 3600 7850 3600
Wire Wire Line
	7850 5000 3900 5000
Wire Wire Line
	3900 5000 3900 6000
Wire Wire Line
	3900 6000 3950 6000
Wire Wire Line
	7850 3600 7850 4250
$Comp
L Device:R R?
U 1 1 5F2CBEBF
P 8250 4850
F 0 "R?" H 8320 4896 50  0000 L CNN
F 1 "2k2" H 8350 4800 50  0000 L CNN
F 2 "" V 8180 4850 50  0001 C CNN
F 3 "~" H 8250 4850 50  0001 C CNN
	1    8250 4850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5F2CBEC5
P 8250 4400
F 0 "R?" H 8320 4446 50  0000 L CNN
F 1 "1k" H 8320 4355 50  0000 L CNN
F 2 "" V 8180 4400 50  0001 C CNN
F 3 "~" H 8250 4400 50  0001 C CNN
	1    8250 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 4550 8250 4600
Wire Wire Line
	7850 4250 8250 4250
Wire Wire Line
	7850 4600 8250 4600
Wire Wire Line
	7850 4600 7850 5000
Connection ~ 8250 4600
Wire Wire Line
	8250 4600 8250 4700
$Comp
L power:GND #PWR?
U 1 1 5F30BFB9
P 8250 5000
F 0 "#PWR?" H 8250 4750 50  0001 C CNN
F 1 "GND" H 8255 4827 50  0000 C CNN
F 2 "" H 8250 5000 50  0001 C CNN
F 3 "" H 8250 5000 50  0001 C CNN
	1    8250 5000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F30EAC3
P 6100 2950
F 0 "#PWR?" H 6100 2700 50  0001 C CNN
F 1 "GND" H 6105 2777 50  0000 C CNN
F 2 "" H 6100 2950 50  0001 C CNN
F 3 "" H 6100 2950 50  0001 C CNN
	1    6100 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1250 4350 1950 4350
$Comp
L Transistor_BJT:BD139 Q?
U 1 1 5F3E3A24
P 2950 1950
F 0 "Q?" H 3142 1996 50  0000 L CNN
F 1 "BD139" H 3142 1905 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-126-3_Vertical" H 3150 1875 50  0001 L CIN
F 3 "http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/DATASHEET/CD00001225.pdf" H 2950 1950 50  0001 L CNN
	1    2950 1950
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C?
U 1 1 5F23E40C
P 850 3950
F 0 "C?" H 968 3996 50  0000 L CNN
F 1 "47µ" H 968 3905 50  0000 L CNN
F 2 "" H 888 3800 50  0001 C CNN
F 3 "~" H 850 3950 50  0001 C CNN
	1    850  3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  3800 850  3550
Wire Wire Line
	850  3550 1250 3550
Wire Wire Line
	1400 1950 2750 1950
Wire Wire Line
	1400 1950 1400 3550
$Comp
L Regulator_Linear:LM7805_TO220 U?
U 1 1 5F4BD8D9
P 4450 3500
F 0 "U?" H 4450 3742 50  0000 C CNN
F 1 "LM7805_TO220" H 4450 3651 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 4450 3725 50  0001 C CIN
F 3 "http://www.fairchildsemi.com/ds/LM/LM7805.pdf" H 4450 3450 50  0001 C CNN
	1    4450 3500
	1    0    0    -1  
$EndComp
Connection ~ 7100 2950
Wire Wire Line
	7100 2950 7100 2700
$Comp
L Regulator_Switching:LM2595T-12 U?
U 1 1 5F23EBB7
P 4450 2250
F 0 "U?" H 4450 2617 50  0000 C CNN
F 1 "StepDown 12V" H 4450 2526 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-5_P3.4x3.7mm_StaggerOdd_Lead3.8mm_Vertical" H 4500 2000 50  0001 L CIN
F 3 "http://www.ti.com.cn/cn/lit/ds/symlink/lm2595.pdf" H 4450 2250 50  0001 C CNN
	1    4450 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 2950 3850 2950
Wire Wire Line
	4450 2550 4450 2950
Connection ~ 4450 2950
Wire Wire Line
	4450 2950 6100 2950
Wire Wire Line
	4950 1700 4950 2150
Wire Wire Line
	4950 2150 5150 2150
Wire Wire Line
	5150 2150 5150 3150
Wire Wire Line
	5150 3150 4150 3150
Wire Wire Line
	4150 3150 4150 3500
Connection ~ 4950 2150
Wire Wire Line
	4750 3500 5050 3500
Connection ~ 5700 3500
Wire Wire Line
	4450 3800 3850 3800
Wire Wire Line
	3850 3800 3850 2950
Connection ~ 3850 2950
Wire Wire Line
	3850 2950 4450 2950
$Comp
L Device:C C?
U 1 1 5F507691
P 5050 3650
F 0 "C?" H 5165 3696 50  0000 L CNN
F 1 "100 nF" H 5165 3605 50  0000 L CNN
F 2 "" H 5088 3500 50  0001 C CNN
F 3 "~" H 5050 3650 50  0001 C CNN
	1    5050 3650
	1    0    0    -1  
$EndComp
Connection ~ 5050 3500
Wire Wire Line
	5050 3500 5400 3500
$Comp
L Device:CP C?
U 1 1 5F509FC6
P 5400 3650
F 0 "C?" H 5518 3696 50  0000 L CNN
F 1 "10 uF" H 5518 3605 50  0000 L CNN
F 2 "" H 5438 3500 50  0001 C CNN
F 3 "~" H 5400 3650 50  0001 C CNN
	1    5400 3650
	1    0    0    -1  
$EndComp
Connection ~ 5400 3500
Wire Wire Line
	5400 3500 5700 3500
Wire Wire Line
	4450 3800 5050 3800
Connection ~ 4450 3800
Wire Wire Line
	5400 3800 5050 3800
Connection ~ 5050 3800
Connection ~ 6100 2950
Wire Wire Line
	6100 2950 7100 2950
Wire Wire Line
	7100 2950 7100 3700
Wire Wire Line
	7100 1500 7100 2700
Wire Wire Line
	3150 2950 3150 4350
Connection ~ 3150 4350
Wire Wire Line
	3150 4350 3150 6900
Wire Wire Line
	7100 1050 7900 1050
Wire Wire Line
	7350 1700 7350 1400
Wire Wire Line
	4950 1700 7350 1700
$Comp
L Device:R R?
U 1 1 5F5366D8
P 3500 2150
F 0 "R?" V 3293 2150 50  0000 C CNN
F 1 "30" V 3384 2150 50  0000 C CNN
F 2 "" V 3430 2150 50  0001 C CNN
F 3 "~" H 3500 2150 50  0001 C CNN
	1    3500 2150
	0    1    1    0   
$EndComp
Wire Wire Line
	3650 2150 3950 2150
Wire Wire Line
	3350 2150 3050 2150
Wire Wire Line
	1400 3550 2200 3550
Connection ~ 1400 3550
Wire Wire Line
	2500 3550 2850 3550
Wire Wire Line
	2850 2550 2850 3550
Wire Wire Line
	5700 3500 5700 5150
Wire Wire Line
	1950 4350 3150 4350
Wire Wire Line
	1050 6900 3150 6900
Connection ~ 3150 6900
Wire Wire Line
	3150 6900 3150 7550
$EndSCHEMATC
