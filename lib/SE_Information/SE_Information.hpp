#ifndef SE_INFORMATION_HPP
#define SE_INFORMATION_HPP

/* ================= FEATURE CONFIG ================= */
#define Extension_Enable      1    // 1: Modbus RTU Relay, 0: No extend
#define RTC_Enable            0    // 1: true, 0: false

/* ================= WIFI AP CONFIG ================= */
#define APSSID       "SmartElex-Relay-6CH"
#define APPSK        "12345678"
#define HOSTNAME      "smartelex"

/* ================= RTC TIME CONFIG ================= */
#define RTC_OPEN_Time_Hour    8
#define RTC_OPEN_Time_Min     6
#define RTC_Closs_Time_Hour   16
#define RTC_Closs_Time_Min    30


//---GPIO_Pin_Map---//
#define CH1 '1'                 // CH1 Enabled Instruction
#define CH2 '2'                 // CH2 Enabled Instruction
#define CH3 '3'                 // CH3 Enabled Instruction
#define CH4 '4'                 // CH4 Enabled Instruction
#define CH5 '5'                 // CH5 Enabled Instruction
#define CH6 '6'                 // CH6 Enabled Instruction
#define ALL_ON  '7'             // Start all channel instructions
#define ALL_OFF '8'             // Disable all channel instructions

#endif
