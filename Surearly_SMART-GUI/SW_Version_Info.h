/* ***************** Surearly Smart Manager program version history ***************** */
/* ***** Project: Live version ***** */

/*20210706 / Version: v1.2
- BLE name 설정 최대 길이 수정
	1) 수정 전: 8-byte ("SuSm0000")
	2) 수정 후: 20-byte ("SurearlySmart-000000")
- Serial baud rate 수정
	1) 수정 전: 9,600 bps
	2) 수정 후: 38,400 bps
- Serial의 API message 수신 부 최적화
	1) BLE_Rx_Enqueue()
	2) BLE_Rx_Queue_Process()
- "연결 및 설정"에 통신 속도 설정 tap 추가
    1) 9,600, 38,400 bps 사용자 선택
    2) Baud rate 설정에 따른 BLE name 설정 항목 자동 입력 -> 수정 전 BLE module에 name 길이 8자 이상 설정 방지 용
        (1) 9,600 bps 연결 시: "SuSm0000"
        (2) 38,400 bps 연결 시: "SurearlySmart-000000"
*/


/* 20211026 / v1.3
** 공정 검사의 "블루투스 이름 설정" 기능 제거
*** 기기 wake-up 시 BLE name 자동 설정 진행으로 변경
** 연결 및 설정의 블루투스 이름 수동 설정 기능 추가
*** 기기에서 name 자동 설정 실패 시 GUI program에서 수동 설정 진행
** BLE 연결 해제 상태에서 기능 실행 방지 추가
*** BLE 연결 전 공정, 완제품 검사 및 QA test mode의 기능 실행 시 오류 메시지로 알림
*/

/* 20211111 / v1.4
** 완제품 검사 기능 개선
*** MAC address 표시 기능 추가
*** Serial number 입력 항목 추가
*** Test report 저장 시 file name 자동 입력
**** File name: Test_Report-"Serial number"-"MAC address".csv
*** 항목 초기화 기능 수정
**** 검사 항목 1~4 적합에 check 설정, 항목 5는 초기화
*** 단축키 기능 추가
** 연결 및 설정
*** 블루투스 연결 기능 수정
**** 연결 버튼 누를 시 연결 완료까지 5회 재시도 기능 추가
*/

/* 20220315 / Version: v1.5
- BLE baud rate default 값 변경
	1) 38,400 -> 115,200 bps
- BLE name 설정 오류 확인 및 설정 진행
	1) 생산 모드의 기기 자동 설정 기능에서 BLE name 설정 오류 확인 시 설정 진행 API 명령 ("BLE_NAME") 전송
		(1) Command: BLE_NAME
		(2) Data-key (setup_state): start
		(3) Data-key (name): SurearlySMART-xxxxxx
- BLE master dongle 강제 분리 시 program 멈춤 문제 해결
	1) 원인: BLE master dongle 강제 분리 시 노이즈 발생으로 serial 수신 버퍼로 다량의 data가 수신됨
	2) 해결:
		(1) 1 msec 간격 수신된 data 길이가 5000-byte 이상일 경우 BLE master dongle 강제 분리로 인식
		(2) 수신 buffer data erase 및 작동 중인 기능 중지
- BLE 수신 buffer 처리부 개선
	1) 수정 전: 1 msec 주기로 serial queue buffer에서 1-byte 씩 data 꺼내 (dequeue) 명령 처리
	2) 수정 후: 1 msec 마다 serial queue buffer에 수신 된 message 길이 확인 후 한번에 data를 꺼내 명령 처리
*/

/* 20220407 / Version: v1.6
- Firmware update 기능 구현
    1) Device IAP 모드 진입 API 명령 ("FW_UPD") 전송 및 처리 추가
    2) Firmware binary file 열기 및 정보 확인 기능 추가
    3) Binary file raw data 송수신용 Packet data 처리부 구현
    4) Update 진행 과정 progress bar, percentage 표시 구현
    5) 진행 상황 로그 모니터에 출력
*/

/* 20220816 / Version: v1.7
- Read PTR 기능 추가
    1) LED 밝기에 따른 PTR Data 출력
    2) DAC Value, Optics Channel 선택 가능
*/

/* 20221013 / Version: v1.7_alpha
! Version 표기 오류: v1.7_alpha (X) v1.8_alpha (O)
- 비정상 종료 문제 해결
	1) [QA Test mode - LED 캘리브레이션]에 Stick 삽입 확인 추가
	2) API Parsing에서 json 추출 실패 시 변수 초기화 후 함수 종료
*/

/* 20221019 / Version: v1.7_alpha.1
! Version 표기 오류: v1.7_alpha.1 (X) v1.8_alpha.1 (O)
- BLE 보안 연결 적용
	1) "+PASSKEY" 응답 확인 후 "AT+ACCEPT" 전송
	2) "AT+ACCEPT" 재전송 주기 = 2 sec
*/

/* 20221214 / Version: v1.8.0
- Software Version 관리 규칙에 따라 버전 수정
- '펌웨어 버전' Panel 수정
	1) alpha 버전은 문자열 표시 오류가 발생하여 Panel 크기 조정
- 응용 프로그램 수정
	1) 이름 수정 (Main_UI.exe -> Surearly SMART Manager.exe)
	2) 파일 속성 수정 및 버전 표기
- 배포 파일 수정
	1) 파일 통일화 (Main_UIx.cds -> Distribution_setting.cds)
	2) 설치 파일, 폴더 이름 등 세부항목 수정
*/

/* 20230203 / Version: vT.0.1
- 완제품 검사 - 캘리브레이션 기능 추가
	1) 캘리브레이션 단독 (캘리브레이션 + Read -> X)
*/