/* ***************** Firmware version history ***************** */
/* ***** Project: Live version ***** */

/*2020.02.19 / Version: v1.0.0
  1. Base on firmware: CES2020_FW v1.025
*/
/*2020.02.22 / Version: v1.1.0
  1. Complete API function
   * API command set
    - DEV_INFO; 기기 정보 확인
    - RECENT_RSLT; 최근 측정 결과 전송
    - STAT_CHK; 기기 현재 상태 확인
    - DEV_RST; 테스트 재 진행 명령
    - ALARM_CHK; Stick 점적 확인 alarm
    - SET_LOT_INFO; Stick LOT 정보 입력
*/
/*2020.02.24 / Version: v1.1.1
  1. USER_GPIO_init() 위치 및 BLE 초기 Pin 상태 정의
    * Battery 삽입 시에만  USER_GPIO_init() 진행
      기기가 켜진 이후로 On/Off switch 입력 시 Device_instant_wakeup()에서 USER_GPIO_init() 실행 안함
      => Pin 초기화로 기기 sleep 중 BLE 켜짐 방지
  2. CLK_MainRegulatorCmd(ENABLE) - Device_wakeup(), Device_instant_wakeup() 추가
  3. CLK_HSICmd(DISABLE), CLK_LSICmd(DISABLE) - Device_instant_sleep(), Device_sleep() 추가
  4. Main Process function 추가
    * App Connection Waiting
    * Stick Insert Waiting
    * Stick Check
    * Error Stick Check
    * Sample Load Waiting
    * Sample React Waiting
    * Sample Analyze
    * Result Display
*/
/*2020.02.25 / Version: v1.1.2
  1. API Error 처리 추가 - Length mismatch, Unidentified command
  2. API_Tx_Message() 함수 code 인자값 추가 (정상, 에러 코드)
  3. Adminmode - Preg T1 cutoff data 5개 입력 받도록 기능 추가 (Preg T1 만 6단계 구분, 나머지는 4단계)
  4. Get_Stick_Bandline_Level() 함수 최적화 및 Preg T1 레벨 6단계 구분 추가
*/
/*2020.02.26 / Version: v1.1.3
  1. Operation algorithm 단계에 맞게 API Key-Value 설정
  2. API_Key_Value_Init(), API_Key_Value_Setting() 함수 추가
  3. LCD_Blink_Timer(): Timer3 -> Timer2로 이동 (API 송수신이 많을 경우 LCD 표시 지연 현상 발생)
*/
/*2020.02.27 / Version: v1.1.4
  1. API available proc_step 적용
  2. API DEV_RST command 기능 추가 (change stick, reset)
  3. RTC setting, API DEV_INFO 수신시 시간정보 설정 추가, 측정 완료시 최근 측정 시간 정보 기록은 작업중.
*/
/*2020.02.28 / Version: v1.1.5
  1. Error_Stick_Insert_Check() - 1분내 stick 삽입 없을 경우 기기 자동 종료 추가
  2. API_Set_Calendar() - 최근 측정 시간 셋팅 기능 추가 
*/
/*2020.03.02 / Version: v1.1.6
  1. API를 통해 content cutoff setting시 저장 기능 추가
  2. API (response) data length 값 버그 수정 (길이 부족, -1)
  3. API_Tx_Message() - Error 메시지의 end 구분 ','와 '}' 구분하도록 수정,
  4. memset(BLE_RX.data, 0, sizeof(BLE_RX.data)); 이동
    수정 전: API_Tx_Message() 함수 내
    수정 후: ble_rx_main_process() 함수 내
  5. project - option - Data model: medium으로 변경 (기존: Large)
*/
/*2020.03.02 / Version: v1.1.7
  1. Queue buffer size 변경:
    수정 전: 500
    수정 후: 50
  2. API error message (404, 411, 416): malloc -> 전역 변수 할당으로 변경
  3. heap szie 뱐경:
    수정 전: 0x400
    수정 후: 0x250
  4. stack size 변경:
    수정 전: 0x200
    수정 후: 0x250
*/
/*2020.03.02 / Version: v1.1.8
  1. BLE_Bot_Queue_Process(): !(BLE_USART->SR&0x20) 부분 제거
    !(BLE_USART->SR&0x20): Data is not received
  2. interrupt 우선 순위 변경
    Timer2 (level 3), Timer3 (level 1), Uart1_rx (level 2)
  3. Queue buffer size 변경:
    수정 전: 50
    수정 후: 200
*/
/*2020.03.03 / Version: v1.1.9
  1. JSON message parsing 후 response 전송 시 (API_Tx_Message 함수) printf 사용 안함
    response 할 message timer2에서 1-byte 씩 전송
    임시로 작성 -> 정리 필요
*/
/*2020.03.04 / Version: v1.2.0
  1. JSON message 전송 시 printf 사용 안함 -> API_Tx_Message()내 API_TX_MSG를 통해 전송.
    API_TX_MSG 저장 후 API_TX_FLAG = SET 시 Timer2에서 1-byte 씩 UART tx data 전송.
  2. Cutoff, 측정 결과 저장 시 API_TX_FLAG 확인
    수정 전: if(memory_save_process == SET)
    수정 후: if((memory_save_process == SET)&&(API_TX_FLAG == RESET))
    API response 시 메모리 저장 실행 안함 -> response 완료 시 메모리 저장 진행
  3. 배란 T3 라인 레벨 확인 시 경쟁 반응 -> 일반 레벨 확인으로 변경 
*/
/*2020.03.09 / Version: v1.2.1
  1. 시작 문자 수신시 tact time 기능 추가
    시작 문자 ('|') 수신 부터 200 ms이내 끝 문자 ('#')가 수신되면 정상 메시지 판단
    200 ms 초과시 이전 수신 데이터 무시
  2. 시작 문자('|') 이후 수신 된 문자가 또 시작 문자 ('|') 일 경우 수신 버퍼 초기화
  3. BLE Tx 150-byte 씩 나누어 전송, 간격 100 ms
  4. Battery 삽입 시 잔량 표시 시간 
    수정 전: 3 sec
    수정 후: 1 sec
  5. Sample_Loading_Waiting():
    1) stick 제거, dipping alarm 우선 순위 적용
      우선 순위: Stick 제거 > dipping alarm
    2) Line 2~5 발색 확인
      기존: Line 1 만 발색 확인
      수정: Line 1~5 발색 확인 (Line 2~5 발색X and Line 1만 발색 되어 있을 때 dipping alarm 'ON') 
      v1.2.1에선 2) 기능 제외: Modoric test 진행 시 불필요 추후 정식 버전에서 추가
*/
/*2020.03.15 / Version: v1.2.2
  1. Stack/Heap, Queue size 변경
    1) Stack: 0x250
    2) Heap: 0x300
    3) Queue: 250-byte
  2. ble_rx_main_process() 기능/버그 수정
    1) 수신된 Cmd, data length 확인 후 버퍼 copy 조건 추가
      - buffer overflow 방지
      - 수신 data length 일치하지 않을 시 -> Error mag (code: '416') 전송
    2) proc_step number에 따라 block 시 BLE_RX.data 초기화 처리 누락 -> 함수 마지막에 BLE_RX.data 초기화 추가
    3) SET_LOT_INFO parsing 시 수신된 array의 json_array_clear() 추가 
  3. API_Tx_Message() 수정
    1) 입력 매개 변수 수정
      - String cmd 입력 -> Const cmd 입력 수정
    2) Tx message 구성 시 cmd length 검사 -> buffer size보다 길다면 cmd_strlen = 0 초기화
  4. Device_wakeup() - memset(&MEM_BANK_1, 0, sizeof(MEM_BANK_1)) 추가
    - 이전 사용 data 및 잘못된 data 입력 시 기기 시작 시 초기화 작업 필요
  5. 측정 결과/Cutoff data EEPROM 저장 방법 변경: byte program -> block program
    1) Ram에서 저장 실행
    2) 저장 시간
      수정 전: 약 1.5 sec
      수정 후: 약 12 msec
  6. Stick_Ejection() 버그 수정
    1) stick_ejection_cnt 이후 User_Mode_Variable_Init()으로 계속 0으로 초기화 됨
      -> stick 분리 시 User_Mode_Variable_Init() 진행으로 수정     
*/
/*2020.03.17 / Version: v1.2.3
  1. admin mode 최적화 및 수정
    1) 기능 제거
      - Device time (RTC) setting
      - Receive buffer read
      - One-time test
      - Color test
      - Device current time
    2) 수정
      - Memory bank initializing: user data 영역만 삭제
      - Read Memory bank: Preg T1 cutoff 5개, 나머지 3개 표현 변경, 최근 측정 level data 표시 제거 
      - Multi-time test: 온도 소수점 1자리 표시
      - Device current time: Device information 항목과 병합
    3) 최적화
      - MEM_BANK_2 제거: Calibration, Cutoff setting -> MEM_BANK_1으로 사용
      - 결과 저장/저장 안함 선택 후 MEM_BANK_1 초기화 진행, EEPROM에서 data load 후 MEM_BANK_1 재 설정
      - BLE name setting: ble_name[] 버퍼 제거, MEM_BANK_1.BLE_NAME[]에 직접 저장
  2. App 연결 완료 후 바로 App에서 data 전송 시 BLE_RX.ok = 1에서 0으로 클리어 안되는 문제
    1) BLE connection check 후 연결 완료 시 BLE_RX.ok = 0 클리어 진행 수정
*/
/*2020.03.18 / Version: v1.2.4
  1. Factory reset 기능 추가
    1) Device 사용 중 손상된 data의 save, load로 기기 오작동 방지
    2) Factory reset 진입 방법
      - 기기 전원 'Off' (sleep) 상태에서 전원 버튼 10번 누름 -> 기기 전원 'On'시 진입
      - 진입 방법 추후 변경 가능
  2. 기기 전원 'On' 시 강제 전원 'Off' 조건 추가
    1) Battery 전압 < MCU 구동 전압 (2.6v) => 강제 전원 'Off'  
*/
/*2020.03.18 / Version: v1.2.5
  1. Stick contents 인식 완료 후 측정 중 다른 contents stick 재 삽입 시 에러 처리
    - Get_Stick_Content() 생성
    1) Sample Load Waiting, Sample React Waiting 단계 적용
    2) Stick error 상태로 결과까지 진행 시 저장 및 결과 표시 안함 ('Book' icon 깜박임)
*/
/*2020.03.19 / Version: v1.2.6
  1. MEM_BANK - BLE_NAME buffer size 변경 (9-byte -> 25-byte)
    1) BLE_NAME[BLE_NAME_LEN] -> char BLE_NAME[BLE_NAME_CMD_BUF_SIZE]
  2. Missing braces issue 해결
    1) C-STAT 분석 시 Missing braces 경고 -> 모든 조건문에 {} 추가
*/
/*2020.03.19 / Version: v1.2.7
  1. Stand alone mode 추가: App연동 or 단독 사용 모드 선택 기능
  2. Factory reset시 MEMORY_ADDR_1, MEMORY_ADDR_2 영역 erase
*/
/*2020.03.19 / Version: v1.2.8
  1. Sample react time count 지연 문제
    1) system_process.c 내 모든 시간 카운트는 timer2에서 진행
      TIMER_COUNT_UP_1, TIMER_COUNT_UP_2, TIMER_COUNT_DOWN_1 구조체 변수 사용하여 카운트
    2) Sample react waiting시 LCD display timer2에서 flag setting시 icon display 진행  
*/
/*2020.03.19 / Version: v1.2.9
  1. API 수신 문자열, MEM_BANK_1 data setting시 버퍼 길이 확인 후 data 저장 - 오버플로우 방지
    1) u_strncpy 함수: 목적지 버퍼 사이즈 만큼만 저장 후 마지막에 NULL 문자 채우기
*/
/*2020.03.24 / Version: v1.3.0
  1. Contents level에 따른 LCD 표시 정보 변경
    1) LCD_Result_Display() 함수 내용 수정 - 구글 공유 sheet 호르몬 수치 및 메시지 참고
*/
/*2020.03.25 / Version: v1.3.1
  1. LCD 결과 표시 - 측정 오류 조건 추가
    1) LCD_Result_Display() 내용 수정 및 lcd_result_icon() 추가 - 구글 공유 sheet 호르몬 수치 및 메시지 참고
*/
/*2020.03.27 / Version: v1.3.2
  1. Content reaction time 설정 기능 추가
    1) Preg, Ovul, Meno 개별 설정 가능
    2) Admin mode - Admin_Reacttime_Content_Select(), Admin_Reacttime_Setting() 함수 추가
*/
/*2020.04.07 / Version: v1.3.3
  1. 기능 추가
    1) system_process.c -> Sample_Analyze(): T2 보정 계산 적용
    2) system_process.c -> Sample_Loading_Waiting(): 실 발색 용 dipping 조건 적용
    3) admin_mode.c: content 별 T2 보정 data 설정 적용
    4) admin_mode.c: Memory bank erase시 진행 여부 확인 추가
    5) admin_mode.c -> Read memory bank: T2 adjusted value, Reaction time, Init operation mode 조회 기능 추가
*/
/*2020.04.08 / Version: v1.3.4
  1. Stick Insertion count 기능 추가
    1) device_setup.c -> Stick_insertion_count() 추가
    2) admin_mode.c -> Read memory bank - stick insertion count 조회 추가
    3) EEPROM count 값 저장 address 위치: 0x17A0, 0x17A1  
  2. 최신 임신, 배란 cutoff data 입력
    1) 임신: SCGM20904, 배란: SLHM20904
  3. 임신, 배란 T2 보정 값 입력
    1) 임신: 1600, 배란: 1500
  4. 기기 측정 완료 횟수 조회 기능 추가
    1) admin_mode.c -> Read memory bank
*/
/*2020.04.14 / Version: v1.3.5
  1. Stick_insertion_count() 버그 수정
    1) Stick 삽입 시 스위치 입력 부분 glitch 현상으로 두번 이상 카운트 문제 -> 100 msec delay 추가
*/
/*2020.04.17 / Version: v1.3.6
  1. Stick_Check() 버그 수정
    1) Stick Check: Aging data와 비교, Factory ptr data 비교 없음
    2) Aging data 확인 범위: 80% 이하 시 stick 불량
  2. Factory reset 시 stick 삽입 횟수 초기화 제거
    1) 추후 admin mode에서 stick 삽입 횟수 초기화기능 추가 예정
  3. Sample_Analyze() ptr data 저장 변경
    1) Factory (band) ptr data updata 안함, admin mode setting data 유지
    2) DAC data만 최신 data로 update
  4. DIPPING_ALARM_RATIO 변경
    1) 기존: 97.0, 변경: 85.0
  5. Stick_Insert_Waiting() - stick 삽입 확인 완료 시 delay 시간 변경
    1) 기존: 1 sec, 변경: 2 sec
  6. 임신, 갱년 T3 초기 cutoff data 변경
    1) 변경 전: 1000,1000,1000 -> 변경 후: 1050,1050,1050
  7. BAND_NOR_RATIO 변경
    1) Sample 전개 확인 시점에서 line1에서 line2 전개가 빠르게 일어날 경우 Dipping 완료 조건과 맞지 않을 수 있으므로, line2 ~ 5의 RATIO를 줄임
*/
/*2020.04.24 / Version: v1.3.7
  1. LED_Calibration() operation 수정
	  1) 루프제한 N (횟수) 수정: 100 -> 20
	  2) x_delta 수정: 20 -> 10
	  3) y0 값 검사: y0가 5000 ~ 33000 범위 내 오도록, x0를 20%씩 이동
	  4)DAC 범위 제한 수정
		  수정 전: 300 < x < 3000
		  수정 후: 500 < x <= 4095
  2. Stick_insertion_count()
    1) stick 삽입 인식 시 delay 수정: 100 ms -> 500 ms
    2) count 횟수 65000 제한
  3, LED_Calibration() 내 printf 주석 처리
    1) API 통신 시 문제 발생      
*/
/*2020.05.08 / Version: v1.3.8
  1. Vref (ADC) turn on settling time 수정 - Vref_Read_ADC()
    1) 수정 전: 2 ms, 수정 후: 5 ms
  2. Wireless firmware updata 진행 확인 flag 주소 macro 선언
    1) 수정 전: 0x17FF 주소값 직접 입력, 수정 후: CHECK_W_FIRMWARE_UPDATE_FLAG 로 macro 선언 후 사용
*/
/*2020.05.25 / Version: v1.3.9
  1. 기능 추가 - Admin mode data 설정 시 factory data 영역에 저장 기능 개선
    1) 수정 전 저장 data
      - LED calibration
    2) 수정 후
      - LED calibration data
      - Cutoff data
      - Correction factor
      - Reaction time
      - BLE name
    3) Factory reset 진행 시 admin mode에서 설정 data로 복구 됨

  2. 수정 사항
    1) 변수명 수정
      - TESTER_DISP_OFF -> ADMIN_MENU_DISPLAY_OFF
      - TESTER_DISP_ON -> ADMIN_MENU_DISPLAY_ON
      - STAND_ALONE_MODE-> INIT_OPERATION_MODE
      - MAIN_MODE -> USER_MODE
      - tester_rx_conplete -> admin_rx_complete
      - tester_init_menu_disp_flag -> admin_init_menu_disp_flag

    2) 함수 추가
      - Run_Usermode(): admin mode 진입 상태에서 |usermode# 명령 시 실행
      - Run_Adminmode(): user mode 진입 상태에서 |adminmode# 명령 시 실행
*/
/*2020.06.01 / Version: v1.3.10
  1. 함수, 변수 명 정리
*/
/*2020.06.03 / Version: v1.3.11
  1. Factory reset 진입 방법 수정
    1) 수정 전: Eject button 10회 누른 후 기기 전원 'On'
    2) 수정 후: 기기 sleep 상태에서 Eject button 15초 누름
      - Eject button 누르고 있는 상태에서 기기 전원 'On'
      - 기기 전원 'On' 후 11초 이후 Factory reset 진행
      - Factory reset 진행 완료 후 LCD icon 전체 3번 깜박임
  2. Stick Lot version 조회 기능 추가
    1) Admin mode - Admin_Read_Memory_Bank() 조회 기능 추가
  3. Stick 삽입 대기 시간 수정: STICK_INSERT_WATIT_TIME
    1) 수정 전: 2분
    2) 수정 후: 5분  
*/
/*2020.08.12 / Version: v1.3.12
  1. XPRIZE Rapid Coivd mode 추가
    1) Admin mode
      - Cutoff data setting - Covid 설정 추가 (T2 설정은 의미 없음, T1, T3 설정만 유효함)
      - React time setting - Covid 설정 추가
      - Read memory bank - Covid cutoff, react time data 표시 추가
    2) User mode
      - Stick_Check - Covid mode 경우 흰색 stick만 인식, 다른 색상은 error 처리
      - Sample_Analyze - Covid mode 경우 발색 위치가 다르므로 covid용 계산 식 추가
    3) JSON protocol - RSLT_DATA 명령 추가
      - 전 채널 (Ch1 ~ 5) 측정 rawdata, T1 ~ 3 계산 rawdata 전송
*/

/*2020.08.21 / Version: v1.3.13
  1. Production mode 추가 (Surearly Smart UI program 연동)
    1) API protocol 추가
      - PRODUCT_MODE: Production mode 진행, 종료
      - CALIB_STAT: LED calibration 진행, 상태 확인
      - CORR_FACTOR: Correction factor setting
      - REACT_TIME: react time setting
      - BLE_NAME: BLE name setting
      - RAW_DATA: Stick sensing raw data
    2) Production_mode.c
      - Production mode 진행 구현
    3) QA mode
      - Standard stick의 raw data sensing 과정 구현
  
  2. Stick 재 삽입 시간 수정: 측정 중 stick 강제 분리 시 재 삽입 대기 시간
    1) 수정 전: 1분
    2) 수정 후: 2분
*/

/*2020.08.24 / Version: v1.3.14
  1. Code 전체 최적화 및 함수 name 정리
*/

/*2020.08.31 / Version: v1.3.15
  1. Xprize Covid mode 진행 수정
    1) App 연결 없이 단독 진행 변경
  2. Covid용 cutoff data, react time setting API 추가
    1) Covid lot version은 "COVTxxxxx" 시작
*/

/*2020.09.01 / Version: v1.3.16
  1. Red color 인식 CONTENT_COLOR_CUTOFF_1 값 수정
    - 수정 전: 0.5
    - 수정 후: 0.35
  2. Save_Memory_Bank_Block_Program() 저장 block 영역 증가
    1) MEM_BANK의 COVID mode data 추가로 용량 증가, EEPROM 저장 address 증가 필요
      - 수정 전: 0 ~ 1 block 저장
      - 수정 후: 0 ~ 2 block 저장
*/

/*2020.09.08 / Version: v1.3.17
  1. Device mode select API 추가
    - API를 통해 기기 작동 모드 선택
    - Mode: App connection, Stand alone, Covid
  2. Admin mode - 기기 초기 작동 모드 선택 기능 제거
*/

/*2020.09.14 / Version: v1.3.18
  1. Vbat_Sense_End(), Vbat_Sense_Start() 함수 및 호출 제거
    - GPIO init 제거
*/

/*2020.09.15 / Version: v1.3.19
  1. Revision PCB v1.2.3 대응
    - 배터리 삽입 시 BLE Internal Pull-down 'Off' 명령 전송 추가 (main.c)
    - BLE_Bot_Sleep(), BLE UART 'Off' 추가
    - BLE_Bot_Wakeup(), BLE UART 'On' 추가
    - BLE_Bot_Uart_Deinit() 내용 모두 주석 처리
    - BLE_AT_BYP_PORT, PIN 변경 적용
*/

/*2020.09.16 / Version: v1.3.20
  1. API "DEV_INFO" Command - lot covid 추가
    
*/

/*2020.09.18 / Version: v1.3.21
  1. API "DEV_INFO" Command
    1) Req: content IgG, Ag 확인 추가
    2) Res: lot_IgG, lot_Ag의 lot version 추가
  2. API "SET_LOT_INFO", "REACT_TIME" COVT (IgG), CAGT (Ag) 추가
  3. App Connection Waiting, Stick Insert Waiting, Sample Loading Waiting 대기 시간 수정
    1) 수정 전: 5분
    2) 수정 후: 10분
  4. COVID Demo Mode Process 수정
    1) COVID mode는 stick content (IgG, Ag)를 기기에서 확인하지 않고, App으로부터 수신
       "DEV_INFO" API를 통해 content정보를 수신, Stick Insert Waiting까지 명령이 정상 수신되야 함.
    2) Error Stick Insert Check: stick 강제 분리 후 재삽입 시 COVID는 컨텐츠 확인 없이 stick color가 white인지 확인 후 정상 진행
  5. 반응 대기 시간 추가
    1) COVID IgG: 8분
    2) COVID Ag: 18분
*/

/*2020.09.22 / Version: v1.3.22
  1. API "RSLT_DATA" Command 사용 범위 변경
    1) 수정 전: Result Display (10)
    2) 수정 후: Sample Load Waiting (7) ~ Result Display (10)
  2. Stick sample 전개 상태 확인 기능 추가 - Sample React Waiting(8) 적용
    1) 1초 간격 stick sensing 후 1 ~ 5 line ratio, T1 ~ 3 rawdata 계산
        API "RSLT_DATA" 명령 수신 시 계산 결과 전송
  3. Error_Stick_Insert_Check 함수 Algorithm 수정 (배터리 소모 개선)
    1) 수정 전: 100 msec 간격 stick sensing -> Stick 전개 상태 확인과 동시 진행 시 높은 배터리 소모
    2) 수정 후: Stick 정상 삽입 상태 stick sensing 진행 안함,
                Stick 강제분리 후 재 삽입 시 stick sensing 진행 (Content 및 재사용 stick 확인)
*/

/*2020.09.22 / Version: v1.3.23
  1. COVID Stick Color 인식 추가
    1) 수정 전: App으로부터 content 정보 수신
    2) 수정 후: Stick color 인식 (IgG - Green color, Ag - Yellow color)
  2. CONTENT_COLOR_CUTOFF_3 변경
    1) 수정 전: 1.7
    2) 수정 후: 1.4
*/

/*2020.09.25 / Version: v1.3.24
  1. "DEV_INFO" API Req의 covid content 확인 제거 
    1) 기존 Stick color 인식으로 content 확인 진행
*/

/*2020.09.25 / Version: v1.3.25
  1. Covid IgG, Ag T1 cutoff 4단계 수정
    1) API "SET_LOT_INFO" Res 3개 cutoff 응답 수정
  2. Device 결과 표시 4단계 수정
*/

/* 20201103 / Version: v1.3.26
- Stick_Insertion_Count() call 위치 수정
 1) 수정 전: 매 stick 삽입 시 마다 stick count 및 eeprom 저장 진행
 2) 수정 후: Stick_Insert_Waiting()내 stick 삽입 시 stick count 및 저장 진행
- EEPROM_Byte_Write()내 인라인 함수 __disable_interrupt(), __enable_interrupt() 추가
- BLE_BUFFER_SIZE 변경
 1) 수정 전: 250
 2) 수정 후: 230
- Function (API_Key_Value_Init, API_Key_Value_Setting, Get_Channel_Ratio_And_Raw_data) 위치 이동
 1) 수정 전: User_Mode.c
 2) 수정 후: API_Message_Ctrl.c
- 사용하지 않는 변수 제거 및 최적화
*/
/* 20201111 / Version: v1.3.27
- Memory, code 최적화 진행
*/
/* 20210203 / Version: v1.3.28
- Smart Duo 측정 기능 추가
*/
/* 20210224 / Version: v2.0.0
- 측정 진행 algorithm, API revision
  1) 기기내 측정 정보 (Lot, Cutoff, content, etc.) 저장 하지 않음 
    API를 통해 실시간 정보 수신 (Stick color, react time) 후 측정 진행
  2) 측정 rawdata만 전송 (cutoff 이용 level 분석 진행 하지 않음), 
    LCD 결과 표시는 API 'RSLT_DATA'의 rslt_level 정보로 표시.
*/
/* 20210318 / Version: v2.0.1
- API 'STAT_CHK' 명령 내 stick_color 정보 추가
 1) 기기에 삽입 된 stick color 정보 전송으로 App에서 선택한 color와 실제 기기내 삽입된 stick color 일치 여부 확인 용
- API_DATA_TypeDef내 사용하지 않는 변수 제거
*/
/* 20210421 / Version: v2.0.2
- Error_Stick_Insert_Check() 함수 내 stick 강제 분리 후 재 삽입 시 stick_state = 0 변수 초기화 추가
  증상: Sample React Waiting (proc_step = 8)에서 stick 강제 분리 후 천천히 재 삽입 과정에서 "stick_state = err_used" 상태가 발생하고,
  끝까지 정상 삽입에도 err_used 상태 유지 문제
  원인: err_used 발생 후 stick 상태 재 확인 시 stick_state 변수 초기화 누락으로 err_used 상태 유지
  해결: Stick 상태 확인 전 stick_state = 0 변수 초기화 과정 추가
*/
/* 20210510 / Version: v2.0.3
- Production mode의 RAW_DATA data 기존 stick sensing rawdata -> ratio 계산 결과로 전송
*/
/* 20210518 / Version: v2.0.4
- White sitck color 인식 문제로 CONTENT_COLOR_CUTOFF_# 범위 수정
*/
/* 20210602 / Version: v2.0.5
- Stick 삽입 확인 switch 인식 후 지연 시간을 2초 -> 3초 수정
*/
/* 20210706 / Version: v2.0.6
- BLE name 길이 (setting/load) 최대 20자 적용
- BLE uart buadrate 9,600 bps -> 38,400 bps 변경
- BLE 수신 부 최적화 (BLE_Bot_Queue_Process)
- API message parsing 처리 주기 (timer) 100 msec -> 5 msec 수정
- Interrupt priority 변경
 1) 변경 전: UART 수신부 (Level-2), API 수신부 (Level-3), API parsing 부 (Level-1)
 2) 변경 후: UART 수신부 (Level-3), API 수신부 (Level-2), API parsing 부 (Level-2)
- 1 sec timer 4 추가 (priority level-3)
 1) 진행 시간 count 전용 timer (TIM 4)
*/
/* 20211019 / v2.0.8 (Beta verison)
- BLE_Bot_Sleep() -> BLE_Bot_Wakeup()으로 function name 변경
- BLE_Bot_Name_Setting() 함수의 입력 및 반환 기능 추가
- Send_AT_Command() 함수 최적화
- BLE_Bot_Queue_Process()의 BLE_RX.ok, BLE_RX.complete -> BLE_RX.at_cmd_resp_chk, BLE_RX.api_resp_chk 변수 명 변경
- 기기 초기 시작 시 BLE internal pull-down 'Off' 추가 (최초 한번만 실행)
- BLE_Bot_Uart_Deinit() 함수에 BLE_RX_PIN setup 추가: Output open-drain low, Reset status 설정
- Device_instant_sleep, Device_instant_wakeup, Device_wakeup, Device_sleep 함수 수정 및 최적화
- Power switch algorithm 변경
- LCD control function - timer 동기화 block 기능 추가 필요
- Sample react waiting 단계에서 진행 과정 flag 변경의 동기화 및 block 기능 추가 필요
- ADC vref/temperature의 slow -> fast channel 설정 변경
- API name, commad 일치 여부 확인 기능 추가
- API message length 계산 과정 수정
- Calibration 진행의 오차 범위 수정 기존 40 -> 100
- USER_TIMER, TIME_CHECK_TypeDef 구조체 이름 변경 및 통합 -> USER_TIMER
- INCRE_TIME_CHK_1, INCRE_TIME_CHK_2, DECRE_TIME_CHK 구조체 이름 변경 -> TIMER_COUNT_UP_1, TIMER_COUNT_UP_2, TIMER_COUNT_DOWN_1
- USER_TIMER update 동기화 함수 추가: Timer_Count_Setup
- API CMD "PRODUCT_MODE"의 response에 dev_id, fw_ver 추가
- BLE UART auto baudrate 기능 추가
- Factory_Reset_Process() 수정
 1) EEPROM block 방식으로 직접 저장
 2) Battery, BLE icon update flag 초기화 추가
- Memory_Bank_Initialization() 수정
 1) MEM_BANK_1.ACCESS_ADDR_1 = SET 적용 가능하도록 위치 이동
- 반응 대기 시간 수정
 1) App 연결 대기: 30 min -> 15 min
 2) Stick 삽입 대기: 30 min -> 15 min
 3) Sample 점적 대기: 30 min -> 15 min
*/
/* 20211020 / v2.0.8
- 초기 BLE name 설정 변경
 1) 수정 전: SurearlySmart-000000, 수정 후: SurearlySMART-000000
*/

/* 20211026 / v2.0.9
- LCD_Deinitialization() 수정: Sleep 시 소모 전류 증가 issue 해결
 1) LCD_DeInit() 후 10 msec 지연 추가: Icon 잔상 문제 해결
 2) COM0~3, SEG0~4 pin의 output open-drain reset status 설정: LCD에 접촉 시 소비 전류 증가 완화
- BLE name 설정 변경
 1) 기존 GUI program을 이용하여 name 설정에서 MCU wake-up 단계에서 자동 설정으로 변경
 2) "SurearlySMART-xxxxxx"로 설정하며, 'x'는 BLE MAC address의 뒤 6자리
 3) BLE name 20자 설정은 PCB v1.2.5 이상 + 수젠텍 전용 BLE F/W가 적용 된 PCB에서 가능
- BLE 통신 오류 메시지 표시 기능 추가
 1) Condition: 기기에 전원 인가 후 BLE auto baudrate 설정 실패 시 LCD에 오류 상태 출력
 2) 오류 상태 표시: BLE + Book icon 5초간 표시
- DAC init 안정화 시간 추가
 1) DAC_Start() 함수 내 DAC_Cmd(DAC_Channel_1, ENABLE); 실행 후 1 msec의 안정화 시간 추가
- Production mode의 auto setting sequence에서 BLE name 설정 과정 제거 및 name 수동 설정 기능 추가
 1) BLE name 설정은 기기 wake-up 시 자동 설정으로 변경
 2) Wake-up에서 자동 설정 실패 시 GUI에서 수동 설정 가능하도록 기능 추가
- BLE_SetupBaudrate() 함수의 기능 개선
 1) BLE module과 통신 가능한 baudrate를 찾고 target baudrate로 BLE module을 재설정 하도록 기능 추가
*/

/* 20211026 / v2.0.10
- API "DEV_INFO" 명령을 이용하여 stick 삽입 대기 시간 설정 기능 추가
 1) 설정 가능한 시간 범위: 1 ~ 60 sec
*/

/*20211028 / v2.0.10 (Modoric test 제공 기기 임시 F/W)
** 임시 기능 수정 및 추가 (BLE name 자동 설정 기능 issue 개선 전 버전)
- BLE_Bot_Name_Setting() 함수 내 BLE_Disconnect(); 제거
- BLE_Start() 함수 수정
 1) BLE name이 초기값인 ("SurearlySMART-000000") 경우 name 설정 진행, GUI program으로 설정된 상태라면 name 설정 진행하지 않음. 
- Production_BLE_Name_Manual_Setup() 함수 수정
 1) GUI program의 연결 및 설정에서 BLE name 수동 설정으로 name 설정 및 저장 가능하도록 기능 구현
*/

/*20211108 / v2.0.11
- LED_Calibration()의 tol 값 변경
 1) 변경 전: 100
 2) 변경 후: 200
- Get_Channel_Ratio_And_Raw_data()의 rslt_rawdata 검사 기능 추가
 1) nan: -9999.9
 2) inf: 9999.9
 3) Maximum (8888.8) 이상: 8888.8
 4) Minimum (-8888.8) 이하: -8888.8 
- Stick 삽입 대기 시간 설정 값 백업 기능 추가
 1) Stick insert waiting 단계에서 DEV_INFO 명령으로 insert_time 설정
 2) 발색 완료 stick 또는 다른 content stick 삽입 시 stick 오류 표시 및 분리 진행
 3) 분리 후 stick insert waiting 단계 재 진입 시 이전 DEV_INFO 명령 설정 값 다시 load 후 이후 단계 진행
*/

/*20211110 / v2.0.12
- BLE name 자동 설정 기능 추가: BLE_Name_Auto_Setting()
 1) BLE 통신 실패 또는 수신 정보 오류 발생 시 retry 과정 추가
 2) 기기 전원 wakeup 시 마다 name 설정 진행 여부 확인
 3) 통신 오류로 name 설정 실패 시 LCD 오류 표시 기능 추가 
*/

/*20211111 / v2.0.13
- 기기 Wake-up에서 battery 저전압 확인 event 처리 수정
 1) 수정 전: 저전압일 경우 기기 강제 종료
 2) 수정 후: 저전압일 경우 battery 'Low' icon 5초간 'On' 후 기기 강제 종료 
- Battery 삽입 후 초기 작동 시 기기 상태 확인 sequence 수정
 1) 수정 전: BLE baud rate 자동 설정 -> power supply test (Battery 잔량 표시, LED Ch1~7 'On') 진행
 2) 수정 후: Power supply test -> BLE baud rate 자동 설정 순서 변경 및 오류 표시
    (1) BLE 통신 정상: Ch6 Green LED 500 msec 'On'
    (2) BLE 통신 불량: Ch7 Red LED 5 sec 'On'
- MEM_BANK의 ACCESS_ADDR_2, BLE_NAME 변수 제거
- BLE_BUFFER_SIZE, QUE_LEN 수정
 1) 수정 전: 320-byte
 2) 수정 후: 240-byte
*/

/*20211117 / v2.0.14
- LED calibration 초기값 재설정
 1) Color line PTR target value 수정: 20,000 -> 30,000
 2) DAC 초기값 변경: 1,000 -> 700
 3) DAC 값 제한 범위 수정: 500 ~ 4095 -> 250 ~ 4095
*/

/*20220315 / v2.0.15
- Stick_Check() 함수 진행 과정 최적화
- 변수 명 변경: stick_error -> stick_error_check
- Sample_Loading_Waiting, Sample_React_Waiting 단계에서 stick 강제 분리 후 재 삽입 시
	stick insert wait time delay 적용 (API로 수신된 insert time)
- Error_Stick_Insert_Check 함수 내 stick 강제 분리 확인 시 stick color 정보를 'U' (Unknown)으로 설정
- API_Rx_Message_Parsing()의 수신된 stick_color의 문자열 길이 확인 (정상 길이 == 1)
  1) API를 통해 stick_color = "Red" 수신 시 'R'로 정상 인식하는 문제를 막기 위함
- Vref, Tempsensor의 ADC_SamplingTimeConfig() 설정 시 ADC_Group_FastChannels -> ADC_Group_SlowChannels 변경
- BLE USART baud rate target value 변경
  1) 38,400  -> 19,200 bps
- BLE USART, name 설정 여부 EEPROM 기록
  1) EEPROM address 0x17A2
    (1) Data == 0x00: USART 설정 필요
    (2) Data == 0x01: USART 설정 완료
  2) EEPROM address 0x17A3
    (1) Data == 0x00: BLE name 설정 필요
    (2) Data == 0x01: BLE name 설정 완료
- 수신 API 처리부 최적화 -> STACK 사용량 최소화
  1) 수정 전: API 송신 진행 중 수신된 API 명령의 parsing 진행
  2) 수정 후: API 송신 중 수신된 API block 처리
- STACK size 설정 수정
  1) 1,024-byte -> 1,360-byte
- Production mode 진입, 설정 완료 알림 기능 추가
  1) Production mode 진입 완료: LCD icon square-1, 2, 3, 4 'On'
  2) 기기 자동 설정 시작
    (1) 설정 정상 완료: LCD icon square-1, 2, 3, 4 5회 깜박임
    (2) Calibation 설정 실패: LCD icon square-1, 2, 3, 4 + Eject + Question 5회 깜박임
    (3) BLE name 설정 실패: LCD icon square-1, 2, 3, 4 + Insert + Question 5회 깜박임
    (4) 모두 실패: LCD icon square-1, 2, 3, 4 + Eject + Insert + Question 5회 깜박임
*/

/*20220407 / v2.0.16
- IAP firmware 대응하여 main application 시작 주소 변경
  1) 변경 전: 0x00008000
  2) 변경 후: 0x0000A800
- RAM vector table 적용
  1) IAP, Main application 2개의 program에서 interrupt 처리 및 실행을 위해 RAM을 vector table 참조 영역으로 설정
    (1) RAM vector table 시작 주소: 0x0100
    (2) Main application 시작 전 RAM 영역 vector table update 진행
- IAP mode 실행 API 명령 ("FW_UPD") 처리 추가
  1) "FW_UPD" 명령 수신 시 응답 후 EEPROM address 0x17A4에 0x01 기록
  2) Watch dog event를 이용하여 device software reset 진행 -> reset 후 IAP program 실행
*/

/*2022. 05. 27 / Version: v2.0.17
- BLE_SLEEP_WAKE pin 초기 상태 변경
	1) 수정 전: Push-pull 'Low'
	2) 수정 후: Push-pull 'High'
- BLE sleep, wake-up 과정 최적화
	1) Sleep function
		(1) 수정 전: BLE_SLEEP_WAKE, BLE_RTS pin 'Low' -> 'High'
		(2) 수정 후: BLE_SLEEP_WAKE pin 'High'
	2) Wake-up function
		(1) 수정 전: BLE_SLEEP_WAKE, BLE_RTS pin 'High' -> 'Low'
		(2) 수정 후: BLE_SLEEP_WAKE 'Low'
- BLE host와 연결 해제 시 확인 과정 추가
	1) FCTY_RST pin으로 Host 장치 연결 해제 후 CONN_STAT pin 상태 확인
		(1) 연결 정상 해제: 'High' -> 'Low'
		(2) 연결 해제 실패: 'High' -> 'High'
		2초 간 CONN_STAT pin 상태 확인 후 해제 실패 시 다음 명령 진행 (Time out 기능)
- BLE sleep시 UART 통신 설정 해제 순서 변경
	1) 수정 전: BLE RX, TX pin pull-up 설정 해제 -> BLE sleep mode 진입
	2) 수정 후: BLE sleep mode 진입 완료 -> BLE RX, TX pin pull-up 설정 해제
		(1) Pull-Up 설정 해제 후 floating 방지를 위해 BLE RX, TX pin Open-drain 'Low' 상태 설정
- BLE sleep mode 진입 중 API 전송 방지
	1) MCU UART 송수신 buffer 초기화 및 API 송신부 block 처리
	2) UART 수신 인터럽트 비활성화 (Queue buffer에 data 입력 방지)
- BLE baud rate setting, "AT+INTPULLDOWN=OFF\r" 설정 결과 표시 수정
	1) Baud rate 설정 완료
		(1) "AT+INTPULLDOWN=OFF" 설정 완료
			광학부 LED Ch-6 (Green) 'On'
			LCD 'BLE'
		(2) "AT+INTPULLDOWN=OFF" 설정 실패
			광학부 LED Ch-7 (Red) 'On'
			LCD 'INSERT' + 'BLE' + 'BOOK'
	2) Baud LCD 'EJECT' + 'BLE' + 'BOOK'rate 설정 실패
		(1) 광학부 LED Ch-7 (Red) 'On'
		(2) LCD 'EJECT' + 'BLE' + 'BOOK'
- 초기 부팅 시 BLE sleep 진입 완료 대기 시간 추가
	1) BLE_SLEEP_WAKE_Pin Push-Pull 'High'로 GPIO 초기 설정 시 BLE sleep mode로 진입하므로 sleep 진입 완료까지 100 msec 대기 시간 추가
- BLE Internal pull-down, name 설정 진행 수정
        1) 수정 전: 설정 완료 확인을 "+OK\r" 명령 수신으로 확인
        2) 수정 후: 설정 완료를 "+ADVERTISING\r" 명령 수신으로 확인
          (1) 설정 진행 관련 함수 내용 수정: Send_AT_Command(), BLE_Bot_Queue_Process(), BLE_Bot_Name_Setting()
*/

/*2022. 11. 07 / Version: v2.0.18_alpha
- sprintf 형식 지정자 변경 %d -> %u
        1) 32,767 이상부터 sprintf %d 할 경우 음수로 표기되어 %u로 변경
- LED Settling Time 변경
        1) 5 ms -> 10 ms
- API) Read PTR 기능 추가
        1) DAC Interval, Optics Channel 정보를 받아 LED 밝기에 따른 PTR 데이터 전송
- BLE_TX_TACT_TIME 변경 200 ms -> 3,000 ms
        1) 보안 연결 적용 BLE 모듈 F/W Download 시 Internal Pull-down Off 설정에서 실패하여 TX 전송 주기 변경
- Stack Overflow Issue 해결
        1) API Parsing 동작 동안 UART Disable
		(1) 첨미자 (#) 확인 → UART Disable → API Parsing
		(2) API Parsing 종료 → UART Enable
        2) JSON 추출 실패 시 (rootValue, rootObject == NULL) 함수 종료
*/