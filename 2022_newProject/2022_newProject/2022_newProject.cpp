// 2022_newProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.

#include "framework.h"
#include "2022_newProject.h"
#include "myCharacter.h"
#include "enemyCharacter.h"
#include "Engraving.h"
#include "skillJob.h"

#include "Winuser.h"
#include<stdlib.h>
#include<time.h>
//파일 입출력용 헤더
#include<string>
#include<fstream>

#include<time.h>

#define MAX_LOADSTRING 100

#define IDC_BTN_START 5000  //시작 버튼 ID
#define IDC_BTN_HELP 5001  //도움말 버튼 ID
#define IDC_BTN_EXIT 5002  //종료 버튼 ID

#define IDC_BTN_ATTACK 5003  //공격 버튼 ID
#define IDC_BTN_SKILL1 5004  //스킬1 버튼 ID
#define IDC_BTN_SKILL2 5005  //스킬2 버튼 ID
#define IDC_BTN_SKILL3 5006  //스킬3 버튼 ID

#define IDC_BTN_BANK 5007  //은행 버튼 ID

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

//타이머용
#define TIMER_ID_1      1  //5분 지나면 돈주기

//사용자 변수

RECT myClientRect;  // 사용가능 영역 크기
int gameStarter = 0;  // 게임 시작 유무 확인
int gameStage = 0;  // 게임스테이지
int diceNumber = 1;  // 주사위 숫자
int gameMoney = 150;  // 게임 자본
int enemyRank = 1;  // 적 반복횟수용
int whoWanderer = 0;  // 떠돌이 상인 변수
int treasureNumber = 0;  // 각인 변수
int treasurePlusAtk = 0;  // 각인 이자 변수
BOOL treasurePlusAtkBool = FALSE;
int treasureCritical = 0;  // 각인 약점 공략
int treasureFastFast = 0;  //각인 속전속결
int treasureGetMana = 0;  //각인 마나약탈자
int enemyAtkVar = 0;  //적 공격 이미지(평타, 흡혈 구분 평타1, 흡혈2)
int bankMoney;  //저축된 금액

int whoTurn = 0; // 누구 공격 차례인가
int flagTurn = 1;  // 속공 함수로 들어가는지 확인용 플래그 변수
int flagCritical = 0; // 누구 차례일때 크리티컬인가?(1 플레이어, 2 적)

int mainSkillSet = 0;  //직업 선택
int subSkillSelect = 0;  //어떤 스킬인지

//스테이지 변화용
int myTempAtkBuf = 0;  //내 캐릭 해당 스테이지 공격력 버프
int enemyTempAtkBuf = 0;  //적 캐릭 해당 스테이지 공격력 버프

int myTempFastBuf = 0;  //내 캐릭 해당 스테이지 공속 버프
int enemyTempFastBuf = 0;  //적 캐릭 해당 스테이지 공속 버프

int myTempDefenceBuf = 0;  //내 캐릭 해당 스테이지 방어력 버프
int enemyTempDefenceBuf = 0;  //적 캐릭 해당 스테이지 방어력 버프

//인게임상 속공수치 비교용
int playCharaterFast;
int enemyCharaterFast;

HWND hWndUi;

BOOL drawDice = FALSE;  // 주사위 그려주나?

RECT myCharacterRect;  // 내 캐릭터
RECT enemyCharacterRect;  // 내 캐릭터
RECT warArea;  // 전투 공간

//스킬 모음
///스킬 포인트
int skillPoint = 3;
///기사(패시브)
BOOL knightWarDefence = FALSE;  // 기사 전투 태세
BOOL knightPowerForce = FALSE;  // 기사 위압
BOOL knightBlackOut = FALSE;  // 기사 무력화
///기사(액티브)
BOOL knightDoubleAttack = FALSE;  //기사 더블 어택
BOOL knightIncisiveAttack = FALSE;  //기사 예리한 일격
//기사와 다르게 도적은 이름으로 적지말고 번호로 적기
///도적
BOOL assassinSkillOne = FALSE;  // 약탈꾼
int assassinCriticalUp = 0;  // 약점 노출 배수용
BOOL assassinSkillTwo = FALSE;  // 백어택
BOOL assassinSkillThree = FALSE;  // 잔상 공격
///기술자
BOOL techSkillOne = FALSE;  // CQC
int techRevenge = 0;  // 리벤저 배수용
BOOL techBotOne = FALSE;  // 공격 봇
BOOL techBotTwo = FALSE;  // 응급처치 봇
BOOL techSkillTwo = FALSE;  // 마탄의 사수
BOOL techSkillFour = FALSE;  // 기계 반란

///스킬 마나 확인용
BOOL skill_1_text = FALSE;  //첫번째 스킬 찍었나
BOOL skill_2_text = FALSE;  //두번째 스킬 찍었나
BOOL skill_3_text = FALSE;  //세번째 스킬 찍었나

//사용자 함수
void ResetGameStater(HWND rs_hWnd);
void AttackCharaterAni(HWND hWnd, int flag);
void AttackFastSummary(HWND hWnd);
void SaveStatPoint(HWND statDlghWnd);
void SaveSkillPoint(HWND skillDlghWnd);
void SaveTreasurePoint(HWND treaDlghWnd);
void GameUI(HWND hWnd);
void GameCharacterUI(HWND hWnd, HDC hdc);
int AttackTurn(int myFast, int enemyFast);
void HitByCharater(HWND hWnd, HDC hdc);
void GetSkill(HWND g_hWnd, int skillNumber);
int CriticalHit(int flagNumber);
void AttackButtonClicked(int whoFlag, HWND atkButtonHWND, HDC atkButtonHDC);

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY2022NEWPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2022NEWPROJECT));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2022NEWPROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY2022NEWPROJECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

STARTUPINFO a_si;
PROCESS_INFORMATION a_pi;

HWND gameStartBtn, gameHelpBtn, gameExitBtn, gameBankBtn;
HWND gameStrStatStatic, gameDpStatStatic, gameMoneyStatic;
HWND atkBtn;
HWND skillBtn1, skillBtn2, skillBtn3;

MyCharacter* myMainCharacter = new MyCharacter;
EnemyCharacter* enemyMainCharacter = new EnemyCharacter;
Engraving* engravingStat = new Engraving;
SkillJob* myMainChaCharacterSkill = new SkillJob;

//떠돌이 상인 다이얼로그
BOOL CALLBACK WandererDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    WCHAR wandererText[256] = { 0, };
    HWND tempDlghWnd;

    switch (iMessage) {
    case WM_INITDIALOG:
        SetDlgItemInt(hDlg, 100, 100, FALSE);
        SetDlgItemInt(hDlg, 150, 150, FALSE);
        GameUI(hWndUi);
        
        //떠돌이 상인 등장!
        if (whoWanderer == 1) {
            //체력을 소모하여 힘 스탯 3증가
            wsprintfW(wandererText, L"반갑구만 모험가!\n근육이 그렇게 약해서야 진정한 모험을 떠날수없지\n15체력 저하 물약을 마시면 자네는 더욱 강해질수있는데 어떤가?\n아니면 15원짜리 체력회복 물약도있네\n\n현재 체력 -15\n힘 +3");
            tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_WANDERERTEXT);
            SetWindowText(tempDlghWnd, wandererText);
            whoWanderer = 1;
        }
        else if (whoWanderer == 2) {
            //돈을 소모하여 방어력 스탯 3증가
            wsprintfW(wandererText, L"반갑구만 모험가!\n모험을 떠나는데 충분한 준비는했겠지?\n이 80원짜리 든든한 방패를 산다면 모험에 도움이 될텐데..\n아니면 15원짜리 체력회복 물약도있네\n\n방어력 +3");
            tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_WANDERERTEXT);
            SetWindowText(tempDlghWnd, wandererText);
            whoWanderer = 2;
        }
        else if (whoWanderer == 3) {
            //돈을 소모하여 스킬포인트 1증가
            wsprintfW(wandererText, L"마법학회에서 연구중인 스킬포인트 물약을 가져왔는데\n어때 한번 사보겠나? 250원이라내\n돈이 없다면 15원짜리 체력회복 물약도 있고\n\n스킬포인트 +1");
            tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_WANDERERTEXT);
            SetWindowText(tempDlghWnd, wandererText);
            whoWanderer = 3;
        }
        else if (whoWanderer == 4) {
            //돈을 소모하여 최생 6증가
            wsprintfW(wandererText, L"자네 그런 체력으로 적들을 상대로 버틸수있겠나?\n이 든든한 고기를 먹으면 최대 체력이 6증가하니 한번 먹어보라고\n아 물론 값은 받아야겠지 70원일세\n돈이 없다면 15원짜리 체력회복 물약도 있고\n\n최대, 현재 체력 +6");
            tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_WANDERERTEXT);
            SetWindowText(tempDlghWnd, wandererText);
            whoWanderer = 4;
        }
        return TRUE;
    case WM_COMMAND:
        //처음 버튼 눌렀을때(상인 찬성)
        if (LOWORD(wParam) == IDC_BUTTON_WANDERER1) {
            //처음 상인(체력 -> 공격력)
            if (whoWanderer == 1) {
                if (myMainCharacter->getCurrentHealth() > 15) {
                    myMainCharacter->setDamage(3);
                    myMainCharacter->setCurrentHealth(myMainCharacter->ImRealHit(15));
                }
                else {
                    MessageBox(hDlg, L"큰일날뻔했어!\n자네 체력이 너무 없구만\n아쉽지만 다음에 보지", L"무산된 거래", MB_OK);
                }
            }
            else if (whoWanderer == 2) {
                //두번째 상인(돈 -> 방어력)
                if (gameMoney >= 80) {
                    myMainCharacter->setDefence(3);
                    gameMoney -= 80;
                }
                else {
                    MessageBox(hDlg, L"이봐! 돈도없으면서 거래는 무슨\n돈을 벌고 오게", L"무산된 거래", MB_OK);
                }
            }
            else if (whoWanderer == 3) {
                //세번째 상인(돈 -> 스킬포인트)
                if (gameMoney >= 250) {
                    skillPoint++;
                    gameMoney -= 250;
                }
                else {
                    MessageBox(hDlg, L"이 귀한걸 그냥 달라고하는군", L"무산된 거래", MB_OK);
                }
            }
            else if (whoWanderer == 4) {
                //세번째 상인(돈 -> 체력)
                if (gameMoney >= 70) {
                    myMainCharacter->setMaxHealth((myMainCharacter->getHealth() + 6));
                    myMainCharacter->setCurrentHealth((myMainCharacter->getCurrentHealth() + 6));
                    gameMoney -= 70;
                }
                else {
                    MessageBox(hDlg, L"아무리 그래도 이건 아닌것같군", L"무산된 거래", MB_OK);
                }
            }
        }
        //두번째 물약 버튼눌렀을때(상인 반대)
        else if (LOWORD(wParam) == IDC_BUTTON_WANDERER2) {
            if (gameMoney >= 15) {
                gameMoney -= 15;
                myMainCharacter->setCurrentHealth(myMainCharacter->getCurrentHealth() + 15);
                if (myMainCharacter->getHealth() < myMainCharacter->getCurrentHealth()) {
                    myMainCharacter->setCurrentHealth(myMainCharacter->getHealth());
                }
            }
            else {
                MessageBox(hDlg, L"아무리 싸구려 포션이더라도 포션은 포션이야 돈을 가져오라고", L"무산된 거래", MB_OK);
            }
        }
        //세번째 버튼 지나간다.
        else if (LOWORD(wParam) == IDC_BUTTON_WANDERER3) {
            EndDialog(hDlg, LOWORD(wParam));
        }
        //끝내기
        GameUI(hWndUi);
        EndDialog(hDlg, LOWORD(wParam));
        break;
    }
    return 0;
}

//스탯 다이얼로그
BOOL CALLBACK StatDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    switch (iMessage) {
    case WM_INITDIALOG:
        SetDlgItemInt(hDlg, 100, 100, FALSE);
        SetDlgItemInt(hDlg, 150, 150, FALSE);
        //적용된 스텟 보여주기
        SaveStatPoint(hDlg);
        return TRUE;
    
    case WM_COMMAND:
        //창 닫기
        if (LOWORD(wParam) == IDCANCEL || LOWORD(wParam) == IDCLOSE)
        {
            EndDialog(hDlg, LOWORD(wParam));
        }
        //힘 스텟 찍었을때
        else if (LOWORD(wParam) == IDC_BUTTON_STRUP) {
            if (gameMoney >= engravingStat->costArray[0]) {
                gameMoney -= engravingStat->costArray[0];
                engravingStat->UpStrPoint();
                myMainCharacter->setDamage(1);
            }
            else {
                MessageBox(hDlg, L"돈이 부족합니다!\n스테이지를 진행하며 돈을 획득해주세요", L"더 줘", MB_OK);
            }
        }
        //방어력 스텟 찍었을때
        else if (LOWORD(wParam) == IDC_BUTTON_DPUP) {
            if (gameMoney >= engravingStat->costArray[1]) {
                gameMoney -= engravingStat->costArray[1];
                engravingStat->UpDpPoint();
                myMainCharacter->setDefence(1);
            }
            else {
                MessageBox(hDlg, L"돈이 부족합니다!\n스테이지를 진행하며 돈을 획득해주세요", L"더 줘", MB_OK);
            }
        }
        //체력 스텟 찍었을때
        else if (LOWORD(wParam) == IDC_BUTTON_HPUP) {
            if (gameMoney >= engravingStat->costArray[2]) {
                gameMoney -= engravingStat->costArray[2];
                engravingStat->UpHpPoint();
                myMainCharacter->setHealth(2);
            }
            else {
                MessageBox(hDlg, L"돈이 부족합니다!\n스테이지를 진행하며 돈을 획득해주세요", L"더 줘", MB_OK);
            }
        }
        //속공 스텟 찍었을때
        else if (LOWORD(wParam) == IDC_BUTTON_FASTUP) {
            if (gameMoney >= engravingStat->costArray[3]) {
                gameMoney -= engravingStat->costArray[3];
                engravingStat->UpFastPoint();
                myMainCharacter->setFastAttack(2);
            }
            else {
                MessageBox(hDlg, L"돈이 부족합니다!\n스테이지를 진행하며 돈을 획득해주세요", L"더 줘", MB_OK);
            }
        }
        //크리티컬 스텟 찍었을때
        else if (LOWORD(wParam) == IDC_BUTTON_CRIUP) {
            if (gameMoney >= engravingStat->costArray[4]) {
                gameMoney -= engravingStat->costArray[4];
                engravingStat->UpCriPoint();
                myMainCharacter->setCritical(1);
            }
            else {
                MessageBox(hDlg, L"돈이 부족합니다!\n스테이지를 진행하며 돈을 획득해주세요", L"더 줘", MB_OK);
            }
        }
        //마나 스텟 찍었을때
        else if (LOWORD(wParam) == IDC_BUTTON_MANAUP) {
            if (gameMoney >= engravingStat->costArray[5]) {
                gameMoney -= engravingStat->costArray[5];
                engravingStat->UpManaPoint();
                myMainCharacter->setMana(1);
            }
            else {
                MessageBox(hDlg, L"돈이 부족합니다!\n스테이지를 진행하며 돈을 획득해주세요", L"더 줘", MB_OK);
            }
        }
        SaveStatPoint(hDlg);
        
        
        InvalidateRect(hWndUi, NULL, TRUE);
        UpdateWindow(hWndUi);
        GameUI(hWndUi);
        break;
    }
    return 0;
}

//스킬 다이얼로그
BOOL CALLBACK SkillDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    WCHAR skillPointText[32] = { 0, };
    WCHAR skillText[128] = { 0, };
    HWND tempDlghWnd;
    int check;
    int skillCheck;
    switch (iMessage) {
    case WM_INITDIALOG:
        SetDlgItemInt(hDlg, 100, 100, FALSE);
        SetDlgItemInt(hDlg, 150, 150, FALSE);
        wsprintfW(skillPointText, L"보유중인 스킬 포인트 : %d", skillPoint);
        tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLPOINT);
        SetWindowText(tempDlghWnd, skillPointText);

        wsprintfW(skillPointText, L" ");
        tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
        SetWindowText(tempDlghWnd, skillPointText);

        SaveSkillPoint(hDlg);
        return TRUE;
    case WM_COMMAND:
        //창 닫기
        if (LOWORD(wParam) == IDCANCEL || LOWORD(wParam) == IDCLOSE)
        {
            EndDialog(hDlg, LOWORD(wParam));
        }
        //직업 선택
        if (LOWORD(wParam) == IDC_BUTTON_KNIGHT_0_0 && mainSkillSet == 0) {
            check = MessageBox(hDlg, L"기사 전직\n기사는 한방은 약하지만 든든한 방어력으로 적을 처치하는 스타일을 가진 직업입니다.\nN기사로 전직하여 방어력이 8증가하며 기사 스킬을 배울수있게됩니다.\n다른 직업 스킬은 배울수없게됩니다.\n\n스킬 포인트가 필요합니다.", L"기사", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                skillPoint--;
                myMainCharacter->setDefence(8);
                mainSkillSet = myMainChaCharacterSkill->KnightSkillSelect();
                UpdateWindow(hWndUi);
                InvalidateRect(hWndUi, &warArea, TRUE);
            }
            else if (check == IDOK && skillPoint <= 0){
                MessageBox(hDlg, L"스킬포인트가 필요합니다!", L"기사", MB_OK);
            }
        }
        else if (LOWORD(wParam) == IDC_BUTTON_ASSASSIN_0_0 && mainSkillSet == 0) {
            check = MessageBox(hDlg, L"도적 전직\n도적은 빠른 공격으로 적을 처치하는 스타일을 가진 직업입니다.\n속공 수치가 4증가합니다.\n다른 직업 스킬은 배울수없게됩니다.\n\n스킬 포인트가 필요합니다.", L"도적", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                skillPoint--;
                myMainCharacter->setFastAttack(4);
                mainSkillSet = myMainChaCharacterSkill->AssassinSkillSelect();
                UpdateWindow(hWndUi);
                InvalidateRect(hWndUi, &warArea, TRUE);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(hDlg, L"스킬포인트가 필요합니다!", L"도적", MB_OK);
            }
        }
        else if (LOWORD(wParam) == IDC_BUTTON_TECH_0_0 && mainSkillSet == 0) {
            check = MessageBox(hDlg, L"기술자 전직\n기술자는 봇을 설치해 적과 싸우며 강한 화력으로 적을 처치하는 스타일을 가진 직업입니다.\n속공 수치가 1, 치명타 수치가 3증가합니다.\n다른 직업 스킬은 배울수없게됩니다.\n\n스킬 포인트가 필요합니다.", L"기술자", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                skillPoint--;
                myMainCharacter->setFastAttack(1);
                myMainCharacter->setCritical(3);
                mainSkillSet = myMainChaCharacterSkill->TechnicianSkillSelect();
                UpdateWindow(hWndUi);
                InvalidateRect(hWndUi, &warArea, TRUE);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(hDlg, L"스킬포인트가 필요합니다!", L"기술자", MB_OK);
            }
        }
        //기사이면
        if (mainSkillSet == 1) {
            //기사 스킬 0
            if (LOWORD(wParam) == IDC_BUTTON_KNIGHT_1_1) {
                skillCheck = myMainChaCharacterSkill->GetPlayerSelectSkill(0);
                if (skillCheck == 0) {
                    GetSkill(hDlg, 0);
                }
                else {
                    wsprintfW(skillText, L"상대방의 공격력이 자신의 방어력보다 낮을경우 상대방에게 방어력에 영향을 받는 피해량 5를 줍니다.");
                    tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
                    SetWindowText(tempDlghWnd, skillText);
                }
            }
            //기사 스킬 1
            else if (LOWORD(wParam) == IDC_BUTTON_KNIGHT_1_2) {
                skillCheck = myMainChaCharacterSkill->GetPlayerSelectSkill(1);
                if (skillCheck == 0) {
                    GetSkill(hDlg, 1);
                }
                else {
                    wsprintfW(skillText, L"적의 속공 수치를 3낮춥니다.");
                    tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
                    SetWindowText(tempDlghWnd, skillText);
                }
            }
            //기사 스킬 2
            else if (LOWORD(wParam) == IDC_BUTTON_KNIGHT_1_3) {
                skillCheck = myMainChaCharacterSkill->GetPlayerSelectSkill(2);
                if (skillCheck == 0) {
                    GetSkill(hDlg, 2);
                }
                else {
                    wsprintfW(skillText, L"자신의 속공 수치가 2낮아지지만 공격력이 6증가합니다.");
                    tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
                    SetWindowText(tempDlghWnd, skillText);
                }
            }
            //기사 스킬 3
            else if (LOWORD(wParam) == IDC_BUTTON_KNIGHT_2_1) {
                skillCheck = myMainChaCharacterSkill->GetPlayerSelectSkill(3);
                if (skillCheck == 0) {
                    GetSkill(hDlg, 3);
                }
                else {
                    wsprintfW(skillText, L"해당 스테이지에서 자신의 공격력이 4증가합니다.");
                    tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
                    SetWindowText(tempDlghWnd, skillText);
                }
            }
            //기사 스킬 4
            else if (LOWORD(wParam) == IDC_BUTTON_KNIGHT_2_2) {
                skillCheck = myMainChaCharacterSkill->GetPlayerSelectSkill(4);
                if (skillCheck == 0) {
                    GetSkill(hDlg, 4);
                }
                else {
                    wsprintfW(skillText, L"주사위 *2 + 공격력의 공격을 가합니다.");
                    tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
                    SetWindowText(tempDlghWnd, skillText);
                }
            }
            //기사 스킬 5
            else if (LOWORD(wParam) == IDC_BUTTON_KNIGHT_2_3) {
                skillCheck = myMainChaCharacterSkill->GetPlayerSelectSkill(5);
                if (skillCheck == 0) {
                    GetSkill(hDlg, 5);
                }
                else {
                    wsprintfW(skillText, L"주사위 *4 + 공격력의 공격을 가합니다.");
                    tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
                    SetWindowText(tempDlghWnd, skillText);
                }
            }
        }
        //도적이면
        else if (mainSkillSet == 2) {
            //도적 스킬 0
            if (LOWORD(wParam) == IDC_BUTTON_ASSASSIN_1_1) {
                skillCheck = myMainChaCharacterSkill->GetPlayerSelectSkill(0);
                if (skillCheck == 0) {
                    GetSkill(hDlg, 0);
                    
                }
                else {
                    wsprintfW(skillText, L"상대방 처치시 돈을 5원 더 획득합니다.");
                    tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
                    SetWindowText(tempDlghWnd, skillText);
                }
            }
            //도적 스킬 1
            else if (LOWORD(wParam) == IDC_BUTTON_ASSASSIN_1_2) {
                skillCheck = myMainChaCharacterSkill->GetPlayerSelectSkill(1);
                if (skillCheck == 0) {
                    GetSkill(hDlg, 1);
                    
                }
                else {
                    wsprintfW(skillText, L"자신의 속공 수치가 5증가합니다.");
                    tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
                    SetWindowText(tempDlghWnd, skillText);
                }
            }
            //도적 스킬 2
            else if (LOWORD(wParam) == IDC_BUTTON_ASSASSIN_1_3) {
                skillCheck = myMainChaCharacterSkill->GetPlayerSelectSkill(2);
                if (skillCheck == 0) {
                    GetSkill(hDlg, 2);
                    
                }
                else {
                    wsprintfW(skillText, L"자신의 치명타 확률이 5% 증가합니다.");
                    tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
                    SetWindowText(tempDlghWnd, skillText);
                }
            }
            //도적 스킬 3(아드레날린)
            else if (LOWORD(wParam) == IDC_BUTTON_ASSASSIN_2_1) {
                skillCheck = myMainChaCharacterSkill->GetPlayerSelectSkill(3);
                if (skillCheck == 0) {
                    GetSkill(hDlg, 3);
                    
                }
                else {
                    wsprintfW(skillText, L"현재 체력의 5이 감소하지만 현재 스테이지에서 속공이 5증가합니다.");
                    tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
                    SetWindowText(tempDlghWnd, skillText);
                }
            }
            //도적 스킬 4
            else if (LOWORD(wParam) == IDC_BUTTON_ASSASSIN_2_2) {
                skillCheck = myMainChaCharacterSkill->GetPlayerSelectSkill(4);
                if (skillCheck == 0) {
                    GetSkill(hDlg, 4);
                    
                }
                else {
                    wsprintfW(skillText, L"2의 추가공격력을 얻고 해당 공격의 치명타 피해량이 2배 -> 3배로 증가합니다.");
                    tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
                    SetWindowText(tempDlghWnd, skillText);
                }
            }
            //도적 스킬 5
            else if (LOWORD(wParam) == IDC_BUTTON_ASSASSIN_2_3) {
                skillCheck = myMainChaCharacterSkill->GetPlayerSelectSkill(5);
                if (skillCheck == 0) {
                    GetSkill(hDlg, 5);
                    
                }
                else {
                    wsprintfW(skillText, L"주사위 * (자신의 속공 수치 / 10) + 공격력의 공격을 가합니다.");
                    tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
                    SetWindowText(tempDlghWnd, skillText);
                }
            }
        }
        //기술자면
        else if (mainSkillSet == 3) {
            //기술자 스킬 0
            if (LOWORD(wParam) == IDC_BUTTON_TECH_1_1) {
                skillCheck = myMainChaCharacterSkill->GetPlayerSelectSkill(0);
                if (skillCheck == 0) {
                    GetSkill(hDlg, 0);

                }
                else {
                    wsprintfW(skillText, L"공격력이 6감소하지만 일반 공격시 2대 공격합니다.");
                    tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
                    SetWindowText(tempDlghWnd, skillText);
                }
            }
            //기술자 스킬 1
            else if (LOWORD(wParam) == IDC_BUTTON_TECH_1_2) {
                skillCheck = myMainChaCharacterSkill->GetPlayerSelectSkill(1);
                if (skillCheck == 0) {
                    GetSkill(hDlg, 1);

                }
                else {
                    wsprintfW(skillText, L"공격시 추가로 힘 스탯 / 2 만큼 피해를 줍니다.\nCQC 미적용");
                    tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
                    SetWindowText(tempDlghWnd, skillText);
                }
            }
            //기술자 스킬 2
            else if (LOWORD(wParam) == IDC_BUTTON_TECH_1_3) {
                skillCheck = myMainChaCharacterSkill->GetPlayerSelectSkill(2);
                if (skillCheck == 0) {
                    GetSkill(hDlg, 2);

                }
                else {
                    wsprintfW(skillText, L"체력이 20미만인 상태로 다음 스테이지로 넘어가면 체력 10회복합니다.");
                    tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
                    SetWindowText(tempDlghWnd, skillText);
                }
            }
            //기술자 스킬 3
            else if (LOWORD(wParam) == IDC_BUTTON_TECH_2_1) {
                skillCheck = myMainChaCharacterSkill->GetPlayerSelectSkill(3);
                if (skillCheck == 0) {
                    GetSkill(hDlg, 3);

                }
                else {
                    wsprintfW(skillText, L"다음 공격시 적의 방어력을 무시하는 자신의 공격력-2의 추가 공격을 가합니다.");
                    tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
                    SetWindowText(tempDlghWnd, skillText);
                }
            }
            //기술자 스킬 4
            else if (LOWORD(wParam) == IDC_BUTTON_TECH_2_2) {
                skillCheck = myMainChaCharacterSkill->GetPlayerSelectSkill(4);
                if (skillCheck == 0) {
                    GetSkill(hDlg, 4);

                }
                else {
                    wsprintfW(skillText, L"해당 스테이지에서 받는 피해가 1감소하고\n피해를 받으면 상대방에게 방어력에 영향을 받는 피해량 2를 줍니다.\n중첩");
                    tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
                    SetWindowText(tempDlghWnd, skillText);
                }
            }
            //기술자 스킬 5
            else if (LOWORD(wParam) == IDC_BUTTON_TECH_2_3) {
                skillCheck = myMainChaCharacterSkill->GetPlayerSelectSkill(5);
                if (skillCheck == 0) {
                    GetSkill(hDlg, 5);

                }
                else {
                    wsprintfW(skillText, L"(주사위 + 설치된 장비 개수)* (공격력 - 6)의 공격을 가합니다.");
                    tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLHELPTEXT);
                    SetWindowText(tempDlghWnd, skillText);
                }
            }
        }
        SaveSkillPoint(hDlg);
        wsprintfW(skillPointText, L"보유중인 스킬 포인트 : %d", skillPoint);
        tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLPOINT);
        SetWindowText(tempDlghWnd, skillPointText);
        
        InvalidateRect(hWndUi, NULL, TRUE);
        UpdateWindow(hWndUi);
        GameUI(hWndUi);
        break;
    }
    return 0;
}

//각인 다이얼로그
RECT treasureRect;

BOOL CALLBACK TreasureDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    /*
    /// 각인 설명
    -1 판금갑옷 : 판금갑옷 : 방어력에 영향을 받는 최종적인 피해가 4감소합니다.
    -2 이자 : 각인을 얻고 10번째 공격마다 추가 피해 10+(적 강화수치 *2)를 줍니다.
    -3 회복약 : 전투가 끝나면 체력을 10회복합니다.(최대 체력이라면 5원을 회복합니다.)
    -4 여신의 가호 : 방어력 / 5 만큼 추가 방어력을 제공합니다.
    -5 속전속결 : 공격력이 2, 속공 수치가 2증가합니다.
    -6 약점 공략 : 치명타 확률이 5%증가합니다.
    -7 사냥의 시간 : 체력이 50% 이하인 적 공격시 주는 피해가 20% 증가합니다.
    -8 마나 약탈자 : 기본 공격시 마나를 2회복합니다.
    */
    HDC MemDC;
    HBITMAP myBitmap, oldBitmap;
    WCHAR treasureNeedMoneyText[32] = { 0, };
    HWND tempDlghWnd;
    
    switch (iMessage) {
    case WM_INITDIALOG:
        SetDlgItemInt(hDlg, 150, 150, FALSE);

        GetClientRect(hDlg, &treasureRect);

        wsprintfW(treasureNeedMoneyText, L"각인 변환 비용 30원");
        tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_TREASUREMONEY);
        SetWindowText(tempDlghWnd, treasureNeedMoneyText);
        SaveTreasurePoint(hDlg);

        InvalidateRect(hDlg, NULL, TRUE);
        UpdateWindow(hDlg);
        return TRUE;

    case WM_COMMAND:
        //창 닫기
        if (LOWORD(wParam) == IDCANCEL || LOWORD(wParam) == IDCLOSE)
        {
            EndDialog(hDlg, LOWORD(wParam));
        }
        //변환 눌렀을때
        else if (LOWORD(wParam) == IDC_BUTTON_TREASURESELECT) {
            if (gameMoney >= 30) {
                srand((unsigned int)time(NULL));
                gameMoney -= 30;
                treasureFastFast = 0;  // 속전속결 버프 초기화
                treasureCritical = 0;  // 약점 공략 버프 초기화
                treasureGetMana = 0;  // 마나 약탈자 버프 초기화
                treasureNumber = (rand() % 8) + 1;
                SaveTreasurePoint(hDlg);
            }
            else {
                MessageBox(hWndUi, L"각인 변환을 위한 금액이 부족합니다.", L"각인", MB_OK);
            }
        }
        UpdateWindow(hDlg);
        InvalidateRect(hDlg, NULL, TRUE);

        InvalidateRect(hWndUi, NULL, TRUE);
        UpdateWindow(hWndUi);
        GameUI(hWndUi);
        break;

        //각인 이미지 그려주기
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hDlg, &ps);
        MemDC = CreateCompatibleDC(hdc);
        //각인 이미지
        myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITCARD_1));  //초기화
        if (treasureNumber == 1) {
            myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_TREASURE_1_1));
        }
        else if (treasureNumber == 2) {
            myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_TREASURE_2_1));
        }
        else if (treasureNumber == 3) {
            myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_TREASURE_3_1));
        }
        else if (treasureNumber == 4) {
            myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_TREASURE_4_1));
        }
        else if (treasureNumber == 5) {
            myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_TREASURE_5_1));
        }
        else if (treasureNumber == 6) {
            myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_TREASURE_6_1));
        }
        else if (treasureNumber == 7) {
            myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_TREASURE_7_1));
        }
        else if (treasureNumber == 8) {
            myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_TREASURE_8_1));
        }
        oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
        BitBlt(hdc, (treasureRect.left) + 20, (treasureRect.top + 30), 60, 100, MemDC, 0, 0, SRCCOPY);  //비트맵 그려주기
        SelectObject(MemDC, oldBitmap);
        DeleteObject(myBitmap);

        EndPaint(hWndUi, &ps);
    }
    break;
    }
    return 0;
}
int gameRemember[2] = { 0, }; //게임 킬때 돈 주기, 게임 킬때 각인 기억
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    HDC MemDC;
    HBITMAP myBitmap, oldBitmap;

    //파일용 변수
    ofstream writeFile;
    ifstream readFile;
    string strFile;
    int saveCheck;

        //treasureNumber
    int rememberI;
    char rememberArr[8];

    //떠상 물약
    int enemyModel = gameStage % 5;
    int getSkillStage = gameStage % 10;
    int check;

    switch (message)
    {
    case WM_CREATE:
    {
        gameStage = 0;
        //파일 읽어오기
        readFile.open("gameResetMe.txt");
        if (readFile.is_open()) {
            rememberI = 0;
            while (!readFile.eof()) {
                readFile.getline(rememberArr, 8);
                gameRemember[rememberI] = stoi(rememberArr);
                rememberI++;
            }
            readFile.close();
        }
        else {
            writeFile.open("gameResetMe.txt");
            strFile = to_string((gameStage / 5)) + "\n";
            writeFile.write(strFile.c_str(), strFile.size());
            strFile = to_string(treasureNumber);
            writeFile.write(strFile.c_str(), strFile.size());
        }
        hWndUi = hWnd;
        SetWindowPos(hWnd, NULL, 500, 300, 1024, 768, 0);  // 게임창 크기 조절
        GetClientRect(hWnd, &myClientRect);  // 조절된 크기 가져오기
        gameStartBtn = CreateWindow(L"button", L"게 임  시 작", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            (myClientRect.left + 50), 150, 150, 100, hWnd, (HMENU)IDC_BTN_START, NULL, NULL);
        gameHelpBtn = CreateWindow(L"button", L"도 움 말", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            (myClientRect.left + 50), 300, 150, 100, hWnd, (HMENU)IDC_BTN_HELP, NULL, NULL);
        gameExitBtn = CreateWindow(L"button", L"종    료", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            (myClientRect.left + 50), 450, 150, 100, hWnd, (HMENU)IDC_BTN_EXIT, NULL, NULL);
        gameBankBtn = CreateWindow(L"button", L"은행", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            (myClientRect.right-50), 50, 30, 30, hWnd, (HMENU)IDC_BTN_BANK, NULL, NULL);
        ShowWindow(gameBankBtn, SW_HIDE);
        playCharaterFast = 0;
        enemyCharaterFast = 0;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDC_BTN_START:
                hdc = GetDC(hWnd);
                //환생을 했다면
                if (gameRemember[0] != 0 || gameRemember[1] != 0) {
                    int resetCheck;
                    resetCheck = MessageBox(hWnd, L"이전에 환생한 정보가 있습니다. 가지고 시작하시겠습니까?", L"게임시작", MB_OKCANCEL);
                    if (resetCheck == IDOK) {
                        gameMoney += gameRemember[0] * 30;
                        treasureNumber = gameRemember[1];
                    }
                }
                SetTimer(hWnd, TIMER_ID_1, 300000, NULL);
                gameStarter = 1;
                gameStage++;
                enemyMainCharacter->EnemyUnit(gameStage, enemyRank);
                ShowWindow(gameStartBtn, SW_HIDE);
                ShowWindow(gameHelpBtn, SW_HIDE);
                ShowWindow(gameExitBtn, SW_HIDE);
                atkBtn = CreateWindow(L"button", L"공격", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    (myClientRect.right - 200), 550, 100, 100, hWnd, (HMENU)IDC_BTN_ATTACK, NULL, NULL);
                //스킬 버튼 일단 가려주고 찍으면 보이게하기
                skillBtn1 = CreateWindow(L"button", L"스킬1", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    (myClientRect.right - 850), 550, 120, 100, hWnd, (HMENU)IDC_BTN_SKILL1, NULL, NULL);

                skillBtn2 = CreateWindow(L"button", L"스킬2", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    (myClientRect.right - 700), 550, 120, 100, hWnd, (HMENU)IDC_BTN_SKILL2, NULL, NULL);

                skillBtn3 = CreateWindow(L"button", L"스킬3", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    (myClientRect.right - 550), 550, 120, 100, hWnd, (HMENU)IDC_BTN_SKILL3, NULL, NULL);

                ShowWindow(skillBtn1, SW_HIDE);
                ShowWindow(skillBtn2, SW_HIDE);
                ShowWindow(skillBtn3, SW_HIDE);

                whoTurn = AttackTurn(myMainCharacter->getFastAttack(), enemyMainCharacter->getFastAttack());

                myCharacterRect.left = 20;
                myCharacterRect.top = 350;
                myCharacterRect.right = 100;
                myCharacterRect.bottom = 500;

                enemyCharacterRect.left = myClientRect.right - 300;
                enemyCharacterRect.top = 350;
                enemyCharacterRect.right = myClientRect.right - 100;
                enemyCharacterRect.bottom = 500;

                warArea.left = myClientRect.left;
                warArea.right = myClientRect.right;
                warArea.top = 190;
                warArea.bottom = 520;

                GameUI(hWnd);
                InvalidateRect(hWnd, &warArea, TRUE);
                ReleaseDC(hWnd, hdc);
                break;
            case IDC_BTN_HELP:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDC_BTN_BANK:
                gameMoney += bankMoney;
                bankMoney = 0;
                ShowWindow(gameBankBtn, SW_HIDE);
                InvalidateRect(hWnd, NULL, TRUE);
                UpdateWindow(hWnd);
                GameUI(hWnd);
                break;
            case IDC_BTN_ATTACK: {
                //플레이어 평타
                if (whoTurn == 1) {
                    hdc = GetDC(hWnd);
                    if (treasureGetMana == 1) {
                        //마나 회복
                        myMainCharacter->setCurrentMana(myMainCharacter->getCurrentMana() + 2);
                        if (myMainCharacter->getCurrentMana() >= myMainCharacter->getMana()) {
                            myMainCharacter->setCurrentMana(myMainCharacter->getMana());
                        }
                    }
                    AttackButtonClicked(1, hWnd, hdc);
                    ReleaseDC(hWnd, hdc);
                }
                //적 공격
                if (whoTurn == 2 && gameStarter == 1) {
                    hdc = GetDC(hWnd);
                    AttackButtonClicked(2, hWnd, hdc);
                    ReleaseDC(hWnd, hdc);
                }
            }
                break;
            case IDC_BTN_SKILL1: {
                //마나 확인(2)
                if (myMainCharacter->getCurrentMana() >= 2) {
                    myMainCharacter->setCurrentMana(myMainCharacter->getCurrentMana() - 2);
                    if (whoTurn == 1 && mainSkillSet == 1) {
                        flagCritical = 0;
                        myTempAtkBuf += 4;
                        whoTurn = AttackTurn(myMainCharacter->getFastAttack(), enemyMainCharacter->getFastAttack());
                        flagTurn = 0;
                    }
                    else if (whoTurn == 1 && mainSkillSet == 2) {
                        flagCritical = 0;
                        if (myMainCharacter->getCurrentHealth() > 5) {
                            myTempFastBuf += 5;
                            myMainCharacter->setCurrentHealth(myMainCharacter->ImRealHit(5));
                            whoTurn = AttackTurn(myMainCharacter->getFastAttack(), enemyMainCharacter->getFastAttack());
                            InvalidateRect(hWnd, &warArea, TRUE);
                            UpdateWindow(hWnd);  //윈도우 업데이트로 인게임 플레이어(캐릭터 위) 체력 최신화
                        }
                        else {
                            MessageBox(hWnd, L"체력이 부족하여 스킬사용이 불가능합니다.\n아드레날린은 체력을 5소모합니다.", L"사용불가", MB_OK);
                        }
                        flagTurn = 0;
                    }
                    else if (whoTurn == 1 && mainSkillSet == 3) {
                        flagCritical = 0;
                        techSkillTwo = TRUE;
                        whoTurn = AttackTurn(myMainCharacter->getFastAttack(), enemyMainCharacter->getFastAttack());
                        flagTurn = 0;
                    }
                    //적 공격
                    if (whoTurn == 2 && gameStarter == 1) {
                        hdc = GetDC(hWnd);
                        AttackButtonClicked(2, hWnd, hdc);
                        ReleaseDC(hWnd, hdc);
                    }
                    //버프로 인한 화면 무효화
                    InvalidateRect(hWnd, NULL, TRUE);
                    UpdateWindow(hWnd);
                    GameUI(hWnd);
                }
                //마나 없음
                else {
                    MessageBox(hWnd, L"마나가 부족합니다!", L"사용불가", MB_OK);
                }
            }

                break;
            case IDC_BTN_SKILL2: {
                //마나 확인(5)
                if (myMainCharacter->getCurrentMana() >= 5) {
                    myMainCharacter->setCurrentMana(myMainCharacter->getCurrentMana() - 5);
                    if (whoTurn == 1 && mainSkillSet == 1) {
                        hdc = GetDC(hWnd);
                        knightDoubleAttack = TRUE;
                        AttackButtonClicked(1, hWnd, hdc);
                        ReleaseDC(hWnd, hdc);
                    }
                    else if (whoTurn == 1 && mainSkillSet == 2) {
                        hdc = GetDC(hWnd);
                        assassinSkillTwo = TRUE;
                        AttackButtonClicked(1, hWnd, hdc);
                        ReleaseDC(hWnd, hdc);
                    }
                    else if (whoTurn == 1 && mainSkillSet == 3) {
                        flagCritical = 0;
                        techRevenge++;
                        whoTurn = AttackTurn(myMainCharacter->getFastAttack(), enemyMainCharacter->getFastAttack());
                        flagTurn = 0;
                    }
                    //적 공격
                    if (whoTurn == 2 && gameStarter == 1) {
                        hdc = GetDC(hWnd);
                        AttackButtonClicked(2, hWnd, hdc);
                        ReleaseDC(hWnd, hdc);
                    }
                    //버프로 인한 화면 무효화
                    InvalidateRect(hWnd, NULL, TRUE);
                    UpdateWindow(hWnd);
                    GameUI(hWnd);
                }
                //마나 없음
                else {
                    MessageBox(hWnd, L"마나가 부족합니다!", L"사용불가", MB_OK);
                }
            }
                break;
            case IDC_BTN_SKILL3: {
                //마나 확인(8)
                if (myMainCharacter->getCurrentMana() >= 8) {
                    myMainCharacter->setCurrentMana(myMainCharacter->getCurrentMana() - 8);
                    if (whoTurn == 1 && mainSkillSet == 1) {
                        hdc = GetDC(hWnd);
                        knightIncisiveAttack = TRUE;
                        AttackButtonClicked(1, hWnd, hdc);
                        ReleaseDC(hWnd, hdc);
                    }
                    else if (whoTurn == 1 && mainSkillSet == 2) {
                        hdc = GetDC(hWnd);
                        assassinSkillThree = TRUE;
                        AttackButtonClicked(1, hWnd, hdc);
                        ReleaseDC(hWnd, hdc);
                    }
                    else if (whoTurn == 1 && mainSkillSet == 3) {
                        flagCritical = 0;
                        techSkillFour = TRUE;
                        whoTurn = AttackTurn(myMainCharacter->getFastAttack(), enemyMainCharacter->getFastAttack());
                        flagTurn = 0;
                    }
                    //적 공격
                    if (whoTurn == 2 && gameStarter == 1) {
                        hdc = GetDC(hWnd);
                        AttackButtonClicked(2, hWnd, hdc);
                        ReleaseDC(hWnd, hdc);
                    }
                }
                //마나 없음
                else {
                    MessageBox(hWnd, L"마나가 부족합니다!", L"사용불가", MB_OK);
                }
            }
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDSTAT:
                if (gameStarter == 1) {
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_Engraving_E), hWnd, StatDlgProc);
                }
                break;
            case IDSKILL:
                if (gameStarter == 1) {
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_SKILL_K), hWnd, SkillDlgProc);
                }
                break;
            case IDTREASURE:
                if (gameStarter == 1 && (gameStage >= 20 || treasureNumber>=1)) {
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_TREASURE_P), hWnd, TreasureDlgProc);
                }
                else {
                    MessageBox(hWnd, L"20스테이지 이후 해금됩니다!", L"각인", MB_OK);
                }
                break;
            case IDRESETGAMESTARTER:
                if (gameStarter == 1) {
                    int resetCheck;
                    resetCheck = MessageBox(hWnd, L"환생한다면 지금까지 진행한 스테이지, 스텟, 스킬을 모두 잃게되지만\n(진행한 스테이지/5)*30의 초기 자금을 받고 1스테이지로 돌아갑니다.\n정말 환생하겠습니까?", L"환생", MB_OKCANCEL);
                    if (resetCheck == IDOK) {
                        ResetGameStater(hWnd);
                    }
                }
                break;
            case IDGAMESAVE:
                saveCheck = MessageBox(hWnd, L"지금까지 진행한 스테이지만큼의 내용을 저장하여 게임을 재시작할때\n 초기 자금을 얻을수있습니다.", L"환생", MB_OKCANCEL);
                if (saveCheck == IDOK) {
                    //파일 쓰기
                    writeFile.open("gameResetMe.txt");
                    strFile = to_string((gameStage / 5)) + "\n";
                    writeFile.write(strFile.c_str(), strFile.size());
                    strFile = to_string(treasureNumber);
                    writeFile.write(strFile.c_str(), strFile.size());
                    writeFile.close();
                }
                break;
            case IDM_EXIT:
            case IDC_BTN_EXIT:
                saveCheck = MessageBox(hWnd, L"지금까지 진행한 스테이지만큼의 내용을 저장하여 게임을 재시작할때\n 초기 자금을 얻을수있습니다.", L"환생", MB_OKCANCEL);
                if (saveCheck == IDOK) {
                    //파일 쓰기
                    writeFile.open("gameResetMe.txt");
                    strFile = to_string((gameStage / 5)) + "\n";
                    writeFile.write(strFile.c_str(), strFile.size());
                    strFile = to_string(treasureNumber);
                    writeFile.write(strFile.c_str(), strFile.size());
                    writeFile.close();
                }
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        case WM_TIMER:
            if (TIMER_ID_1 == wParam && gameStarter == 1 && enemyModel != 0) {
                KillTimer(hWnd, TIMER_ID_1);                    // 1번을 정지
                SetTimer(hWnd, TIMER_ID_1, 300000, NULL);

                ShowWindow(gameBankBtn, SW_SHOW);

                GameUI(hWnd);
            }
            break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            hdc = BeginPaint(hWnd, &ps);

            MemDC = CreateCompatibleDC(hdc);
            //공격시 주사위 보이게하기
            if (drawDice == TRUE && gameStarter == 1) {
                myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITDICE_0));  //초기화
                if (diceNumber == 1) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITDICE_1));
                }
                else if (diceNumber == 2) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITDICE_2));
                }
                else if (diceNumber == 3) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITDICE_3));
                }
                else if (diceNumber == 4) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITDICE_4));
                }
                else if (diceNumber == 5) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITDICE_5));
                }
                else if (diceNumber == 6) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITDICE_6));
                }
                oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
                BitBlt(hdc, (myClientRect.right / 2 - 150), 200, 100, 100, MemDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                SelectObject(MemDC, oldBitmap);
                DeleteObject(myBitmap);
                //치명 or 크리티컬 텍스트 출력
                if (flagCritical == 1) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_CRITICALMENT));
                    oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
                    BitBlt(hdc, myCharacterRect.left, myCharacterRect.top - 80, 100, 50, MemDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                    SelectObject(MemDC, oldBitmap);
                    DeleteObject(myBitmap);
                }
                else if (flagCritical == 2) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_CRITICALMENT));
                    oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
                    BitBlt(hdc, enemyCharacterRect.left, enemyCharacterRect.top - 80, 100, 50, MemDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                    SelectObject(MemDC, oldBitmap);
                    DeleteObject(myBitmap);
                }
                //적 기본 공격, 흡혈 이미지 출력
                if (enemyAtkVar == 1) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_ENEMY_IMG_NORMALATK));
                    
                }
                else if (enemyAtkVar == 2) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_ENEMY_IMG_BLOOD));
                }
                oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
                if (flagCritical == 2) {
                    BitBlt(hdc, enemyCharacterRect.left + 120, enemyCharacterRect.top - 80, 20, 20, MemDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                }
                BitBlt(hdc, enemyCharacterRect.left, enemyCharacterRect.top - 80, 20, 20, MemDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                SelectObject(MemDC, oldBitmap);
                DeleteObject(myBitmap);
                //이자 출력
                if (treasurePlusAtkBool == TRUE) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_TREASURE_2_2));
                    oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
                    BitBlt(hdc, enemyCharacterRect.left+30, enemyCharacterRect.top - 70, 80, 80, MemDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                    SelectObject(MemDC, oldBitmap);
                    DeleteObject(myBitmap);
                    treasurePlusAtkBool = FALSE;
                }
            }
            //공격 끝나면 주사위 안보이게하기
            else if (drawDice == FALSE && gameStarter == 1) {
                myBitmap = LoadBitmap(hInst, MAKEINTATOM(ID_32771));  //초기화
                oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
                BitBlt(hdc, (myClientRect.right / 2 - 150), myClientRect.top + 100, 100, 100, MemDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                SelectObject(MemDC, oldBitmap);
                DeleteObject(myBitmap);
                WCHAR gameStageText[32];
                wsprintfW(gameStageText, L"스테이지 : %d", gameStage);
                TextOut(hdc, (myClientRect.right / 2 - 150), 250, gameStageText, lstrlenW(gameStageText));
            }
            DeleteDC(MemDC);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            
            //게임 시작 + 캐릭 이미지
            if (gameStarter == 1) {
                GameCharacterUI(hWnd, hdc);
                //내 캐릭터 기본 보이기
                MemDC = CreateCompatibleDC(hdc);
                //기사 이미지 보여주기
                if (mainSkillSet == 1) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_KNIGHT1_IMAGE));

                }
                //도적 이미지 보여주기
                else if (mainSkillSet == 2) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_ASSASSIN1_IMAGE));
                }
                //기술자 이미지 보여주기
                else if (mainSkillSet == 3) {
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_GUNNER1_IMAGE));
                }
                //기본 이미지
                else{
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_NOOBIE_IMAGE));
                }
                oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
                BitBlt(hdc, myCharacterRect.left, myCharacterRect.top, 100, 150, MemDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                SelectObject(MemDC, oldBitmap);
                DeleteObject(myBitmap);
                DeleteDC(MemDC);

                //봇 이미지 임시
                ///전투 봇
                if (techBotOne == TRUE) {
                    Rectangle(hdc, myCharacterRect.left + 250, myCharacterRect.top + 70, myCharacterRect.left + 300, myCharacterRect.top+100);
                }
                //치료 봇
                if (techBotTwo == TRUE) {
                    Rectangle(hdc, myCharacterRect.left + 120, myCharacterRect.top -20, myCharacterRect.left + 200, myCharacterRect.top + 10);
                }



                //떠상 물약
                enemyModel = gameStage % 5;
                getSkillStage = gameStage % 10;
                if (enemyModel == 0) {
                    enemyRank++;
                    if (getSkillStage == 0) {
                        MemDC = CreateCompatibleDC(hdc);
                        myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITSKILLPOINT));
                        oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
                        BitBlt(hdc, (myClientRect.right / 2 - 150), 200, 140, 160, MemDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                        SelectObject(MemDC, oldBitmap);
                        DeleteObject(myBitmap);
                        DeleteDC(MemDC);
                        check = MessageBox(hWnd, L"신비로운 힘이 당신 주변에 가까워집니다.\n체력을 15회복합니다.\n스킬 포인트를 획득했습니다.", L"스킬포인트", MB_OK);
                        if (check == IDOK) {
                            skillPoint++;
                            myMainCharacter->setCurrentHealth(myMainCharacter->getCurrentHealth() + 15);
                            if (myMainCharacter->getHealth() < myMainCharacter->getCurrentHealth()) {
                                myMainCharacter->setCurrentHealth(myMainCharacter->getHealth());
                            }
                            myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_BITDICE_0));
                            oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
                            BitBlt(hdc, (myClientRect.right / 2 - 150), 200, 140, 160, MemDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                            SelectObject(MemDC, oldBitmap);
                            DeleteObject(myBitmap);
                            DeleteDC(MemDC);
                        }
                    }
                    else {
                        //어느 상인이 나오나?
                        whoWanderer = (rand() % 4) + 1;
                        //떠상 다이얼로그
                        DialogBox(hInst, MAKEINTRESOURCE(IDD_WANDERER), hWnd, WandererDlgProc);
                    }
                    //다음 스테이지로 넘겨주고 UI 새로고침해주기
                    HitByCharater(hWnd, hdc);
                    GameUI(hWnd);
                }
                //적 모형
                else if (enemyModel == 1 || enemyModel == 3) {
                    MemDC = CreateCompatibleDC(hdc);
                    if (enemyModel == 1) {
                        myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_ENEMYTANKER1));
                        if (gameStage >= 20) {
                            myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_ENEMYTANKER2));
                        }
                    }
                    else if (enemyModel == 3) {
                        myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_ENEMYGUN1));
                        if (gameStage >= 20) {
                            myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_ENEMYGUN2));
                        }
                    }
                }
                else if (enemyModel == 2 || enemyModel == 4) {
                    MemDC = CreateCompatibleDC(hdc);
                    if (enemyModel == 2) {
                        myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_ENEMYFASTDOG1));
                        if (gameStage >= 20) {
                            myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_ENEMYFASTDOG2));
                        }
                    }
                    else if (enemyModel == 4) {
                        myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_ENEMYMACHINE1));
                        if (gameStage >= 20) {
                            myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_ENEMYMACHINE2));
                        }
                    }
                }
                oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
                BitBlt(hdc, enemyCharacterRect.left, enemyCharacterRect.top, 150, 150, MemDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                SelectObject(MemDC, oldBitmap);
                DeleteObject(myBitmap);
                DeleteDC(MemDC);
                //스킬찍으면 마나 몇 까이는지 알려주기
                WCHAR gameManaText[32];
                if (skill_1_text == TRUE) {
                    wsprintfW(gameManaText, L"마나 소모 : 2");
                    TextOut(hdc, (myClientRect.right - 850), 650, gameManaText, lstrlenW(gameManaText));
                }
                if (skill_2_text == TRUE) {
                    wsprintfW(gameManaText, L"마나 소모 : 5");
                    TextOut(hdc, (myClientRect.right - 700), 650, gameManaText, lstrlenW(gameManaText));
                }
                if (skill_3_text == TRUE) {
                    wsprintfW(gameManaText, L"마나 소모 : 8");
                    TextOut(hdc, (myClientRect.right - 550), 650, gameManaText, lstrlenW(gameManaText));
                }
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    WCHAR eDlgBuf[256] = { 0, };
    HWND tempDlghWnd;
    switch (message)
    {
    case WM_INITDIALOG:
        //도움말 팁
        wsprintfW(eDlgBuf, L"적을 처치하며 자원을 얻을수있습니다.\n적은 5스테이지마다 점점 강력해집니다.\n얻은 자원을 토대로 메뉴창에서 스탯을 찍어 더 어려운 적들을 상대할수있습니다.\n매 5스테이지마다 떠돌이 상인이 등장하여 게임에 도움이 되는 물약 또는 체력 회복약을 판매합니다."
            "\n매 10스테이지마다 스킬포인트 물약을 지급하여 스킬을 찍을수있습니다.\n게임 진행이 어렵다면 환생을 이용할수있습니다");
        tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_ABOUTTEXT);
        SetWindowText(tempDlghWnd, eDlgBuf);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

//환생
void ResetGameStater(HWND rs_hWnd) {
    //현재 스테이지의 /5한 값을 구한다.
    //해당 값을 money 변수에 넣는다.
    int money = (gameStage / 5) * 30;
    
    //찍은 스킬들 싹 안보여주기
    ShowWindow(skillBtn1, SW_HIDE);
    ShowWindow(skillBtn2, SW_HIDE);
    ShowWindow(skillBtn3, SW_HIDE);

    //스킬 포인트, 스텟 포인트를 전부 초기화 해준다
    //스테이지를 초기화하고 1스테이지로 돌린다.
    //사용자 변수
    gameStage = 0;  // 게임스테이지
    diceNumber = 1;  // 주사위 숫자
    enemyRank = 1;  // 적 반복횟수용
    whoWanderer = 0;  // 떠돌이 상인 초기화
    //treasureNumber = 0; // 각인 초기화 -> 각인은 초기화 안시켜줌
    treasurePlusAtk = 0; // 이자 초기화
    treasurePlusAtkBool = FALSE; //이자 초기화2
    treasureCritical = 0;  // 약점 공략 초기화
    treasureFastFast = 0;  // 속전속결 초기화
    treasureGetMana = 0;  //마나 약탈자 초기화
    enemyAtkVar = 0;  //적 공격 이미지(평타, 흡혈 구분 평타1, 흡혈2)
    bankMoney;  //저축된 금액

    whoTurn = AttackTurn(myMainCharacter->getFastAttack(), enemyMainCharacter->getFastAttack()); // 누구 공격 차례인가
    flagTurn = 1;  // 속공 함수로 들어가는지 확인용 플래그 변수

    mainSkillSet = 0;  //직업 선택
    subSkillSelect = 0;  //어떤 스킬인지

    //스테이지 변화용
    myTempAtkBuf = 0;  //내 캐릭 해당 스테이지 공격력 버프
    enemyTempAtkBuf = 0;  //적 캐릭 해당 스테이지 공격력 버프

    myTempFastBuf = 0;  //내 캐릭 해당 스테이지 공속 버프
    enemyTempFastBuf = 0;  //적 캐릭 해당 스테이지 공속 버프

    myTempDefenceBuf = 0;  //내 캐릭 해당 스테이지 방어력 버프
    enemyTempDefenceBuf = 0;  //적 캐릭 해당 스테이지 방어력 버프

    drawDice = FALSE;  // 주사위 그려주나?

    //스킬 모음
    ///스킬 포인트
    skillPoint = 0;
    ///기사(패시브)
    knightWarDefence = FALSE;  // 기사 전투 태세
    knightPowerForce = FALSE;  // 기사 위압
    knightBlackOut = FALSE;  // 기사 무력화
    ///기사(액티브)
    knightDoubleAttack = FALSE;  //기사 더블 어택
    knightIncisiveAttack = FALSE;  //기사 예리한 일격
    ///도적
    assassinSkillOne = FALSE;  // 약탈꾼
    assassinCriticalUp = 0;  // 약점 노출 배수용
    assassinSkillTwo = FALSE;  // 백어택
    assassinSkillThree = FALSE;  // 잔상 공격
    /// 기술자
    techSkillOne = FALSE;  // CQC
    techRevenge = 0;  // 리벤저 배수용
    techBotOne = FALSE;  // 공격 봇
    techBotTwo = FALSE;  // 응급처치 봇

    myMainCharacter->Reset();
    enemyMainCharacter->Reset();
    engravingStat->Reset();
    myMainChaCharacterSkill->Reset();
    //gameMoney + money해준다
    gameMoney = 150 + money;  // 게임 자본

    //새로운 적 배치
    gameStage++;
    
    enemyMainCharacter->EnemyUnit(gameStage, enemyRank);

    GameUI(rs_hWnd);
    InvalidateRect(rs_hWnd, NULL, TRUE);
    UpdateWindow(rs_hWnd);
}


void AttackCharaterAni(HWND hWnd, int flag) {
    //flag 1이면 내캐릭터 2면 적 캐릭터 설정하기
    int i = 0;
    int atkDamage = 0;
    if (flag == 1) {
        
        //기사
        if (mainSkillSet == 1) {
            ///기사 액티브 스킬
            //기사 더블 어택
            if (knightDoubleAttack == TRUE) {
                atkDamage = myTempAtkBuf + myMainCharacter->DefaultAttack((diceNumber * 2), myMainCharacter->getDamage());
                knightDoubleAttack = FALSE;
            }
            ///기사 예리한 일격
            else if (knightIncisiveAttack == TRUE) {
                atkDamage = myTempAtkBuf + myMainCharacter->DefaultAttack((diceNumber * 4), myMainCharacter->getDamage());
                knightIncisiveAttack = FALSE;
            }
            else {
                atkDamage = myTempAtkBuf + myMainCharacter->DefaultAttack(diceNumber, myMainCharacter->getDamage());
            }
            ///기사 패시브 스킬
        //기사 무력화
            if (knightBlackOut == TRUE) {
                atkDamage += 6;
            }
        }
        //도적
        else if (mainSkillSet == 2) {
            /// 도적 백어택
            if (assassinSkillTwo == TRUE) {
                atkDamage = myTempAtkBuf + myMainCharacter->DefaultAttack(diceNumber, (myMainCharacter->getDamage() + 2));
            }
            /// 도적 잔상 공격
            else if (assassinSkillThree == TRUE) {
                int tempFastPoint = (int)((myTempFastBuf + myMainCharacter->getFastAttack()) / 10);
                atkDamage = myTempAtkBuf + myMainCharacter->DefaultAttack((diceNumber * tempFastPoint), myMainCharacter->getDamage());
                knightIncisiveAttack = FALSE;
            }
            else {
                atkDamage = myTempAtkBuf + myMainCharacter->DefaultAttack(diceNumber, myMainCharacter->getDamage());
            }
        }
        //기술자
        else if (mainSkillSet == 3) {
            if (techSkillFour == TRUE) {
                int botCount = 0;
                if (techBotOne == TRUE) {
                    botCount++;
                }
                if (techBotTwo == TRUE) {
                    botCount++;
                }
                atkDamage = myTempAtkBuf + myMainCharacter->DefaultAttack((diceNumber + botCount), (myMainCharacter->getDamage() - 6));
                techSkillFour = FALSE;
            }
            else {
                atkDamage = myTempAtkBuf + myMainCharacter->DefaultAttack(diceNumber, myMainCharacter->getDamage());
            }
        }
        else {
            atkDamage = myTempAtkBuf + myMainCharacter->DefaultAttack(diceNumber, myMainCharacter->getDamage());
        }
        
        
        //각인
        //이자 변수 추가하기(내가 때림)
        if (treasureNumber == 2) {
            treasurePlusAtk++;
            //10번이면 추가피해 주기
            if (treasurePlusAtk >= 10) {
                int plusAtk = 10 + (enemyRank * 2);
                enemyMainCharacter->setCurrentHealth(enemyMainCharacter->ImHit(plusAtk));
                treasurePlusAtk = 0;
                treasurePlusAtkBool = TRUE;
            }
        }
        //속전 속결 딜버프
        else if (treasureNumber == 5) {
            atkDamage += 2;
        }

        //크리티컬 확인
        if (flagCritical == 1) {
            if (assassinSkillTwo == TRUE) {
                atkDamage = atkDamage * 3;
            }
            else {
                atkDamage = atkDamage * 2;
            }
        }
        //도적 백어택 초기화
        assassinSkillTwo = FALSE;
        
        //사냥의 시간 적용
        if (treasureNumber == 7 && (enemyMainCharacter->getHealth() /2) >= enemyMainCharacter->getCurrentHealth()) {
            atkDamage = atkDamage * 1.2;
        }
        //마탄의 사수 적용
        if (techSkillTwo == TRUE) {
            enemyMainCharacter->setCurrentHealth(enemyMainCharacter->ImRealHit((atkDamage - 2)));
            techSkillTwo = FALSE;
        }
        else {
            enemyMainCharacter->setCurrentHealth(enemyMainCharacter->ImHit(atkDamage));
        }
        //CQC 추가타
        if (techSkillOne == TRUE) {
            enemyMainCharacter->setCurrentHealth(enemyMainCharacter->ImHit(atkDamage));
        }
        //공격 봇 추가타
        if (techBotOne == TRUE) {
            enemyMainCharacter->setCurrentHealth(enemyMainCharacter->ImHit((engravingStat->strPoint / 2)));
        }
        
        while (i <= 10) {
            myCharacterRect.left += 5;
            myCharacterRect.right += 5;
            InvalidateRect(hWnd, &warArea, TRUE);
            UpdateWindow(hWnd);
            Sleep(10);
            i++;
        }
        i = 0;
        while (i <= 10) {
            myCharacterRect.left -= 5;
            myCharacterRect.right -= 5;
            InvalidateRect(hWnd, &warArea, TRUE);
            UpdateWindow(hWnd);
            Sleep(10);
            i++;
        }
    }
    //죽으면 스킵되게 하기 (컴터 공격
    else if (flag == 2 && enemyMainCharacter->getCurrentHealth() > 0) {
        
        
        //기술자 리벤저
        if (techRevenge >= 1) {
            enemyMainCharacter->setCurrentHealth(enemyMainCharacter->ImHit((techRevenge * 2)));
            atkDamage = enemyTempAtkBuf + enemyMainCharacter->DefaultAttack(diceNumber, enemyMainCharacter->getDamage()) - techRevenge;
        }
        else {
            //평타 기본
            atkDamage = enemyTempAtkBuf + enemyMainCharacter->DefaultAttack(diceNumber, enemyMainCharacter->getDamage());
        }
        
        //크리티컬 확인
        if (flagCritical == 2) {
            atkDamage = atkDamage * 2;
        }
        //기사 전투 태세일때
        if (knightWarDefence == TRUE) {
            if (atkDamage < myMainCharacter->getDefence()) {
                enemyMainCharacter->setCurrentHealth(enemyMainCharacter->ImHit(5));
            }
        }

        //적대 대상 랜덤 변수 넣기
        int enemyDice = 0;
        srand((unsigned int)time(NULL));
        enemyDice = (rand() % 6); //총 6개 패턴 넣기
        ///그중 0~3는 버리는걸로 아무것도 안넣고 4는 체력회복 5는 스킬로 넣어주기(지금은 스킬 없으니 4~5가 회복)
        if (enemyDice >= 4) {
            //흡혈
            enemyAtkVar = 2;
            enemyMainCharacter->setCurrentHealth(enemyMainCharacter->EnemyUnitHeal(enemyRank));
        }
        else {
            //평타
            enemyAtkVar = 1;
        }
        //판금갑옷 각인 적용중
        if (treasureNumber == 1) {
            atkDamage -= 4;
        }
        //여신의 가호
        else if (treasureNumber == 4) {
            myTempDefenceBuf = myMainCharacter->getDefence() / 5;
        }
        myMainCharacter->setCurrentHealth(myMainCharacter->ImHit((atkDamage - myTempDefenceBuf)));
        while (i <= 10) {
            enemyCharacterRect.left -= 5;
            enemyCharacterRect.right -= 5;
            InvalidateRect(hWnd, &warArea, TRUE);
            UpdateWindow(hWnd);
            Sleep(10);
            i++;
        }
        i = 0;
        while (i <= 10) {
            enemyCharacterRect.left += 5;
            enemyCharacterRect.right += 5;
            InvalidateRect(hWnd, &warArea, TRUE);
            UpdateWindow(hWnd);
            Sleep(10);
            i++;
        }
        enemyAtkVar = 0;  //초기화
    }
    
    //잠시 대기!
    Sleep(100);
    drawDice = FALSE;
    InvalidateRect(hWnd, &warArea, TRUE);
    UpdateWindow(hWnd);
}

void AttackFastSummary(HWND hWnd) {
    srand((unsigned int)time(NULL));
    diceNumber = (rand() % 6) + 1;
    drawDice = TRUE;
    AttackCharaterAni(hWnd, whoTurn);
    whoTurn = AttackTurn(myMainCharacter->getFastAttack(), enemyMainCharacter->getFastAttack());
    flagTurn = 0;
}

void SaveStatPoint(HWND statDlghWnd) {
    WCHAR eDlgBuf[32] = { 0, };
    HWND tempDlghWnd;

    //금액 / 힘 / 방어력 / 체력 / 속공 / 치명타
    wsprintfW(eDlgBuf, L"보유한 금액 : %d", gameMoney);
    tempDlghWnd = GetDlgItem(statDlghWnd, IDC_STATIC_MONEYPOINT);
    SetWindowText(tempDlghWnd, eDlgBuf);

    wsprintfW(eDlgBuf, L"%d / cost : %d", engravingStat->strPoint, engravingStat->costArray[0]);
    tempDlghWnd = GetDlgItem(statDlghWnd, IDC_STATIC_STRPOINT);
    SetWindowText(tempDlghWnd, eDlgBuf);

    wsprintfW(eDlgBuf, L"%d / cost : %d", engravingStat->dpPoint, engravingStat->costArray[1]);
    tempDlghWnd = GetDlgItem(statDlghWnd, IDC_STATIC_DPPOINT);
    SetWindowText(tempDlghWnd, eDlgBuf);

    wsprintfW(eDlgBuf, L"%d / cost : %d", engravingStat->hpPoint, engravingStat->costArray[2]);
    tempDlghWnd = GetDlgItem(statDlghWnd, IDC_STATIC_HPPOINT);
    SetWindowText(tempDlghWnd, eDlgBuf);

    wsprintfW(eDlgBuf, L"%d / cost : %d", engravingStat->fastPoint, engravingStat->costArray[3]);
    tempDlghWnd = GetDlgItem(statDlghWnd, IDC_STATIC_FASTPOINT);
    SetWindowText(tempDlghWnd, eDlgBuf);

    wsprintfW(eDlgBuf, L"%d / cost : %d", engravingStat->criPoint, engravingStat->costArray[4]);
    tempDlghWnd = GetDlgItem(statDlghWnd, IDC_STATIC_CRIPOINT);
    SetWindowText(tempDlghWnd, eDlgBuf);
    
    wsprintfW(eDlgBuf, L"%d / cost : %d", engravingStat->manaPoint, engravingStat->costArray[5]);
    tempDlghWnd = GetDlgItem(statDlghWnd, IDC_STATIC_MANAPOINT);
    SetWindowText(tempDlghWnd, eDlgBuf);
}

void SaveSkillPoint(HWND skillDlghWnd) {
    HWND tempDlghWnd;

    //직업 확인용
    if (mainSkillSet == 1) {
        tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_KNIGHT_0_0);
        SetWindowText(tempDlghWnd, L"기사(전직)");
    }
    else if (mainSkillSet == 2) {
        tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_ASSASSIN_0_0);
        SetWindowText(tempDlghWnd, L"도적(전직)");
    }
    else if (mainSkillSet == 3) {
        tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_TECH_0_0);
        SetWindowText(tempDlghWnd, L"기술자(전직)");
        
    }
    //스킬 찍혀있나 확인용
    if (myMainChaCharacterSkill->GetPlayerSelectSkill(0) == 1) {
        if (mainSkillSet == 1) {
            //습득시 버튼 변경
            tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_KNIGHT_1_1);
            SetWindowText(tempDlghWnd, L"전투 태세(습득)");
        }
        else if(mainSkillSet == 2) {
            //습득시 버튼 변경
            tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_ASSASSIN_1_1);
            SetWindowText(tempDlghWnd, L"약탈꾼(습득)");
        }
        else if (mainSkillSet == 3) {
            //습득시 버튼 변경
            tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_TECH_1_1);
            SetWindowText(tempDlghWnd, L"CQC(습득)");
        }
    }
    //
    if (myMainChaCharacterSkill->GetPlayerSelectSkill(1) == 1) {
        if (mainSkillSet == 1) {
            //습득시 버튼 변경
            //습득시 버튼 변경
            tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_KNIGHT_1_2);
            SetWindowText(tempDlghWnd, L"위압(습득)");
        }
        else if (mainSkillSet == 2) {
            //습득시 버튼 변경
            tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_ASSASSIN_1_2);
            SetWindowText(tempDlghWnd, L"재빠른 손놀림(습득)");
        }
        else if (mainSkillSet == 3) {
            //습득시 버튼 변경
            tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_TECH_1_2);
            SetWindowText(tempDlghWnd, L"공격 봇 설치(습득)");
        }
    }
    //
    if (myMainChaCharacterSkill->GetPlayerSelectSkill(2) == 1) {
        if (mainSkillSet == 1) {
            //습득시 버튼 변경
            tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_KNIGHT_1_3);
            SetWindowText(tempDlghWnd, L"무력화(습득)");
        }
        else if (mainSkillSet == 2) {
            //습득시 버튼 변경
            tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_ASSASSIN_1_3);
            SetWindowText(tempDlghWnd, L"약점 포착(습득)");
        }
        else if (mainSkillSet == 3) {
            //습득시 버튼 변경
            tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_TECH_1_3);
            SetWindowText(tempDlghWnd, L"응급처치 봇 설치(습득)");
        }
    }
    //
    if (myMainChaCharacterSkill->GetPlayerSelectSkill(3) == 1) {
        if (mainSkillSet == 1) {
            //습득시 버튼 변경
            tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_KNIGHT_2_1);
            SetWindowText(tempDlghWnd, L"신의 가호(습득)");
        }
        else if (mainSkillSet == 2) {
            //습득시 버튼 변경
            tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_ASSASSIN_2_1);
            SetWindowText(tempDlghWnd, L"아드레날린(습득)");
        }
        else if (mainSkillSet == 3) {
            //습득시 버튼 변경
            tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_TECH_2_1);
            SetWindowText(tempDlghWnd, L"마탄의 사수(습득)");
        }
    }
    if (myMainChaCharacterSkill->GetPlayerSelectSkill(4) == 1) {
        if (mainSkillSet == 1) {
            //습득시 버튼 변경
            tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_KNIGHT_2_2);
            SetWindowText(tempDlghWnd, L"더블 어택(습득)");
        }
        else if (mainSkillSet == 2) {
            //습득시 버튼 변경
            tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_ASSASSIN_2_2);
            SetWindowText(tempDlghWnd, L"백어택(습득)");
        }
        else if (mainSkillSet == 3) {
            //습득시 버튼 변경
            tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_TECH_2_2);
            SetWindowText(tempDlghWnd, L"리벤저(습득)");
        }
    }
    if (myMainChaCharacterSkill->GetPlayerSelectSkill(5) == 1) {
        if (mainSkillSet == 1) {
            //습득시 버튼 변경
            tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_KNIGHT_2_3);
            SetWindowText(tempDlghWnd, L"예리한 일격(습득)");
        }
        else if (mainSkillSet == 2) {
            //습득시 버튼 변경
            tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_ASSASSIN_2_3);
            SetWindowText(tempDlghWnd, L"잔상 공격(습득)");
        }
        else if (mainSkillSet == 3) {
            //습득시 버튼 변경
            tempDlghWnd = GetDlgItem(skillDlghWnd, IDC_BUTTON_TECH_2_3);
            SetWindowText(tempDlghWnd, L"기계 반란(습득)");
        }
    }

}

void SaveTreasurePoint(HWND treaDlghWnd) {
    
    WCHAR treasureText[128] = { 0, };

    HWND tempDlghWnd;

    if (treasureNumber == 0) {
        wsprintfW(treasureText, L"착용중인 각인이 없습니다.");
        tempDlghWnd = GetDlgItem(treaDlghWnd, IDC_STATIC_TREASURETEXT);
        SetWindowText(tempDlghWnd, treasureText);
    }
    else if (treasureNumber == 1) {
        wsprintfW(treasureText, L"판금갑옷 : 방어력에 영향을 받는 최종적인 피해가 4감소합니다.");
        tempDlghWnd = GetDlgItem(treaDlghWnd, IDC_STATIC_TREASURETEXT);
        SetWindowText(tempDlghWnd, treasureText);
    }
    else if (treasureNumber == 2) {
        wsprintfW(treasureText, L"이자 : 각인을 얻고 10번째 공격마다 추가 피해 10+(적 강화수치 *2)를 줍니다.");
        tempDlghWnd = GetDlgItem(treaDlghWnd, IDC_STATIC_TREASURETEXT);
        SetWindowText(tempDlghWnd, treasureText);
    }
    else if (treasureNumber == 3) {
        wsprintfW(treasureText, L"회복약 : 전투가 끝나면 체력을 10회복합니다.(회복량 초과시 5원을 추가로얻습니다.)");
        tempDlghWnd = GetDlgItem(treaDlghWnd, IDC_STATIC_TREASURETEXT);
        SetWindowText(tempDlghWnd, treasureText);
    }
    else if (treasureNumber == 4) {
        wsprintfW(treasureText, L"여신의 가호 : 방어력 / 5 만큼 추가 방어력을 제공합니다.");
        tempDlghWnd = GetDlgItem(treaDlghWnd, IDC_STATIC_TREASURETEXT);
        SetWindowText(tempDlghWnd, treasureText);
    }
    else if (treasureNumber == 5) {
        wsprintfW(treasureText, L"속전속결 : 공격력이 2, 속공 수치가 2증가합니다.");
        treasureFastFast = 2;
        tempDlghWnd = GetDlgItem(treaDlghWnd, IDC_STATIC_TREASURETEXT);
        SetWindowText(tempDlghWnd, treasureText);
    }
    else if (treasureNumber == 6) {
        wsprintfW(treasureText, L"약점 공략 : 치명타 확률이 7%증가합니다.");
        treasureCritical = 7;
        tempDlghWnd = GetDlgItem(treaDlghWnd, IDC_STATIC_TREASURETEXT);
        SetWindowText(tempDlghWnd, treasureText);
    }
    else if (treasureNumber == 7) {
        wsprintfW(treasureText, L"사냥의 시간 : 체력이 50% 이하인 적 공격시 주는 피해가 20% 증가합니다.");
        tempDlghWnd = GetDlgItem(treaDlghWnd, IDC_STATIC_TREASURETEXT);
        SetWindowText(tempDlghWnd, treasureText);
    }
    else if (treasureNumber == 8) {
        wsprintfW(treasureText, L"마나 약탈자 : 기본 공격시 마나를 2회복합니다.");
        treasureGetMana = 1;
        tempDlghWnd = GetDlgItem(treaDlghWnd, IDC_STATIC_TREASURETEXT);
        SetWindowText(tempDlghWnd, treasureText);
    }
    GameUI(hWndUi);

}

void GameUI(HWND hWnd) {
    WCHAR UIText[256] = { 0, };
    HDC hdc = GetDC(hWnd);
    wsprintfW(UIText, L"현재 체력 / 최대 체력 : %d / %d ", myMainCharacter->getCurrentHealth(), myMainCharacter->getHealth());
    TextOut(hdc, 15, 5, UIText, lstrlenW(UIText));
    wsprintfW(UIText, L"현재 마나 / 최대 마나 : %d / %d ", myMainCharacter->getCurrentMana(), myMainCharacter->getMana());
    TextOut(hdc, 15, 25, UIText, lstrlenW(UIText));
    wsprintfW(UIText, L"공격력 : %d + (%d)   방어력 : %d   돈 : %d",(myMainCharacter->getDamage() + treasureFastFast), myTempAtkBuf, myMainCharacter->getDefence(), gameMoney);
    TextOut(hdc, 15, 45, UIText, lstrlenW(UIText));
    ReleaseDC(hWnd, hdc);
    ///->안하는 이유 자꾸 paint 불러와서 떠상일때 버그 유발함
}

void GameCharacterUI(HWND hWnd, HDC hdc) {
    WCHAR str[32];
    //내 캐릭 정보창
    wsprintfW(str, L"체력 : %d  /  마나 : %d", myMainCharacter->getCurrentHealth(), myMainCharacter->getCurrentMana());
    TextOut(hdc, myCharacterRect.left, myCharacterRect.bottom - 40, str, lstrlenW(str));
    wsprintfW(str, L"속공 : %d", (myTempFastBuf + myMainCharacter->getFastAttack() + treasureFastFast));
    TextOut(hdc, myCharacterRect.left, myCharacterRect.bottom - 20, str, lstrlenW(str));
    wsprintfW(str, L"치명타 : %d", (myMainCharacter->getCritical() + assassinCriticalUp + treasureCritical));
    TextOut(hdc, myCharacterRect.left+100, myCharacterRect.bottom - 20, str, lstrlenW(str));
    if (treasureNumber == 2) {
        wsprintfW(str, L"이자 버프 : %d", treasurePlusAtk);
        TextOut(hdc, myCharacterRect.left, myCharacterRect.bottom, str, lstrlenW(str));
    }
    if (treasureNumber == 7 && (enemyMainCharacter->getHealth() / 2) >= enemyMainCharacter->getCurrentHealth()) {
        wsprintfW(str, L"사냥의 시간 적용됨");
        TextOut(hdc, myCharacterRect.left , myCharacterRect.bottom, str, lstrlenW(str));
    }
    //적 캐릭 정보창
    wsprintfW(str, L"체력 : %d", enemyMainCharacter->getCurrentHealth());
    TextOut(hdc, enemyCharacterRect.left, enemyCharacterRect.bottom - 40, str, lstrlenW(str));
    wsprintfW(str, L"방어력 : %d", enemyMainCharacter->getDefence());
    TextOut(hdc, enemyCharacterRect.left + 100, enemyCharacterRect.bottom - 40, str, lstrlenW(str));
    wsprintfW(str, L"공격력 : %d", enemyMainCharacter->getDamage());
    TextOut(hdc, enemyCharacterRect.left, enemyCharacterRect.bottom - 20, str, lstrlenW(str));
    wsprintfW(str, L"속공 : %d", enemyMainCharacter->getFastAttack());
    TextOut(hdc, enemyCharacterRect.left + 100, enemyCharacterRect.bottom - 20, str, lstrlenW(str));
}

int AttackTurn(int myFast, int enemyFast) {
    flagTurn = 1;
    int winFast = 0;  //1이면 플레이어 2면 적
    while (1) {
        if (playCharaterFast >= 100) {
            playCharaterFast = 0;
            winFast = 1;
            break;
        }
        else if (enemyCharaterFast >= 100) {
            enemyCharaterFast = 0;
            winFast = 2;
            break;
        }
        playCharaterFast = playCharaterFast + myFast + myTempFastBuf + treasureFastFast;
        enemyCharaterFast = enemyCharaterFast + enemyFast + enemyTempFastBuf;
        if (knightPowerForce == TRUE) {
            enemyCharaterFast -= 3;
        }
        /*
        if (knightBlackOut == TRUE) {
            playCharaterFast -= 2;
        }
        */
    }
    return winFast;
}

void HitByCharater(HWND hWnd, HDC hdc) {
    //적 사망
    if (enemyMainCharacter->getCurrentHealth() <= 0) {
        gameStage++;
        enemyMainCharacter->EnemyUnit(gameStage, enemyRank);
        if (assassinSkillOne == TRUE) {
            gameMoney += 5;
            bankMoney += 5;
        }
        gameMoney = gameMoney + 10 + (enemyRank * 10);
        bankMoney = bankMoney + 10 + (enemyRank * 10);
        //마나 회복
        myMainCharacter->setCurrentMana(myMainCharacter->getCurrentMana() + 2);
        if (myMainCharacter->getCurrentMana() >= myMainCharacter->getMana()) {
            myMainCharacter->setCurrentMana(myMainCharacter->getMana());
        }
        //치료 봇
        if (techBotTwo == TRUE && myMainCharacter->getCurrentHealth() < 20) {
            myMainCharacter->setCurrentHealth((myMainCharacter->getCurrentHealth() + 10));
        }
        //속공 초기화 + 스테이지 버프 초기화
        enemyCharaterFast = 0;
        playCharaterFast = 0;
        myTempAtkBuf = 0;
        enemyTempAtkBuf = 0;
        myTempFastBuf = 0;
        enemyTempFastBuf = 0;
        myTempDefenceBuf = 0;
        techRevenge = 0;

        //선공 초기화
        whoTurn = AttackTurn(myMainCharacter->getFastAttack(), enemyMainCharacter->getFastAttack());

        //회복약 각인 적용
        if (treasureNumber == 3) {
            myMainCharacter->setCurrentHealth(myMainCharacter->getCurrentHealth() + 10);
            //회복량 초과시
            if (myMainCharacter->getHealth() < myMainCharacter->getCurrentHealth()) {
                myMainCharacter->setCurrentHealth(myMainCharacter->getHealth());
                gameMoney += 5;
            }
            
        }
        // 전체 초기화
        InvalidateRect(hWnd, NULL, TRUE); 
        UpdateWindow(hWnd);

        ReleaseDC(hWnd, hdc);
    }
    //플레이어 사망
    if (myMainCharacter->getCurrentHealth() <= 0) {
        int resetCheck;
        //선공 초기화
        whoTurn = AttackTurn(myMainCharacter->getFastAttack(), enemyMainCharacter->getFastAttack());
        resetCheck = MessageBox(hWnd, L"전투에서 패배하였습니다! 환생하시겠습니까?\n환생한다면 지금까지 진행한 스테이지, 스텟, 스킬을 모두 잃게되지만\n(진행한 스테이지/5)*30의 초기 자금을 받고 1스테이지로 돌아갑니다.\n확인 버튼을 누르면 환생하고 취소 버튼을 누르면 종료됩니다.", L"사망", MB_OKCANCEL);
        if (resetCheck == IDOK) {
            ResetGameStater(hWnd);
        }
        else {
            DestroyWindow(hWnd);
        }
    }
}

void GetSkill(HWND g_hWnd, int skillNumber) {
    int check;
    //기사
    if (mainSkillSet == 1) {
        //기사 스킬 0
        if (skillNumber == 0) {
            check = MessageBox(g_hWnd, L"상대방의 공격력이 자신의 방어력보다 낮을경우\n상대방에게 방어력에 영향을 받는 피해량 5를 줍니다.", L"전투 태세(패시브)", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                knightWarDefence = TRUE;
                skillPoint--;
                myMainChaCharacterSkill->KnightSkill(0);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"기사", MB_OK);
            }
        }
        //기사 스킬 1
        else if (skillNumber == 1) {
            check = MessageBox(g_hWnd, L"적의 속공 수치를 3낮춥니다.", L"위압(패시브)", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                knightPowerForce = TRUE;
                skillPoint--;
                myMainChaCharacterSkill->KnightSkill(1);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"기사", MB_OK);
            }
        }
        //기사 스킬 2
        else if (skillNumber == 2) {
            check = MessageBox(g_hWnd, L"자신의 속공 수치가 2낮아지지만 공격력이 6증가합니다.", L"무력화(패시브)", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                knightBlackOut = TRUE;
                skillPoint--;
                myMainChaCharacterSkill->KnightSkill(2);
                myMainCharacter->setFastAttack(-2);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"기사", MB_OK);
            }
        }
        //기사 스킬 3
        else if (skillNumber == 3) {
            check = MessageBox(g_hWnd, L"해당 스테이지에서 자신의 공격력이 4증가합니다.", L"신의 가호(액티브)", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                skill_1_text = TRUE;
                SetWindowText(skillBtn1, L"신의 가호");
                ShowWindow(skillBtn1, SW_SHOW);
                skillPoint--;
                myMainChaCharacterSkill->KnightSkill(3);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"기사", MB_OK);
            }
        }
        //기사 스킬 4
        else if (skillNumber == 4) {
            check = MessageBox(g_hWnd, L"주사위 *2 + 공격력의 공격을 가합니다.", L"더블 어택(액티브)", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                skill_2_text = TRUE;
                SetWindowText(skillBtn2, L"더블 어택");
                ShowWindow(skillBtn2, SW_SHOW);
                skillPoint--;
                myMainChaCharacterSkill->KnightSkill(4);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"기사", MB_OK);
            }

        }
        //기사 스킬 5
        else if (skillNumber == 5) {
            check = MessageBox(g_hWnd, L"주사위 *4 + 공격력의 공격을 가합니다.", L"예리한 일격(액티브)", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                skill_3_text = TRUE;
                SetWindowText(skillBtn3, L"예리한 일격");
                ShowWindow(skillBtn3, SW_SHOW);
                skillPoint--;
                myMainChaCharacterSkill->KnightSkill(5);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"기사", MB_OK);
            }
        }
    }
    //도적
    else if (mainSkillSet == 2) {
        //도적 스킬 0
        if (skillNumber == 0) {
            check = MessageBox(g_hWnd, L"상대방 처치시 돈을 5원 더 획득합니다.", L"약탈꾼(패시브)", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                assassinSkillOne = TRUE;
                skillPoint--;
                myMainChaCharacterSkill->AssassinSkill(0);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"도적", MB_OK);
            }
        }
        //도적 스킬 1
        else if (skillNumber == 1) {
            check = MessageBox(g_hWnd, L"자신의 속공 수치가 5증가합니다.", L"재빠른 손놀림(패시브)", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                skillPoint--;
                myMainChaCharacterSkill->AssassinSkill(1);
                myMainCharacter->setFastAttack(5);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"도적", MB_OK);
            }
        }
        //도적 스킬 2
        else if (skillNumber == 2) {
            check = MessageBox(g_hWnd, L"자신의 치명타 확률이 5% 증가합니다.", L"약점 포착(패시브)", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                skillPoint--;
                myMainChaCharacterSkill->AssassinSkill(2);
                assassinCriticalUp += 5;
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"도적", MB_OK);
            }
        }
        //도적 스킬 3
        else if (skillNumber == 3) {
            check = MessageBox(g_hWnd, L"현재 체력의 5이 감소하지만 현재 스테이지에서 속공이 5증가합니다.", L"아드레날린(액티브)", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                skill_1_text = TRUE;
                SetWindowText(skillBtn1, L"아드레날린");
                ShowWindow(skillBtn1, SW_SHOW);
                skillPoint--;
                myMainChaCharacterSkill->AssassinSkill(3);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"도적", MB_OK);
            }
        }
        //도적 스킬 4
        else if (skillNumber == 4) {
            check = MessageBox(g_hWnd, L"2의 추가공격력을 얻고 해당 공격의 치명타 피해량이 2배 -> 3배로 증가합니다.", L"백어택(액티브)", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                skill_2_text = TRUE;
                SetWindowText(skillBtn2, L"백어택");
                ShowWindow(skillBtn2, SW_SHOW);
                skillPoint--;
                myMainChaCharacterSkill->AssassinSkill(4);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"도적", MB_OK);
            }

        }
        //도적 스킬 5
        else if (skillNumber == 5) {
            check = MessageBox(g_hWnd, L"주사위 * (자신의 속공 수치의 / 10) + 공격력의 공격을 가합니다.", L"잔상 공격(액티브)", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                skill_3_text = TRUE;
                SetWindowText(skillBtn3, L"잔상 공격");
                ShowWindow(skillBtn3, SW_SHOW);
                skillPoint--;
                myMainChaCharacterSkill->AssassinSkill(5);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"도적", MB_OK);
            }
        }
    }
    //기술자
    else if (mainSkillSet == 3) {
        //기술자 스킬 0
        if (skillNumber == 0) {
            check = MessageBox(g_hWnd, L"공격력이 6감소하지만 일반 공격시 2대 공격합니다.", L"CQC(패시브)", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                techSkillOne = TRUE;
                myMainCharacter->setDamage(-6);
                skillPoint--;
                myMainChaCharacterSkill->TechnicianSkill(0);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"기술자", MB_OK);
            }
        }
        //기술자 스킬 1
        else if (skillNumber == 1) {
            check = MessageBox(g_hWnd, L"공격시 추가로 힘 스탯 / 2 만큼 피해를 줍니다.\nCQC 미적용", L"공격 봇 설치(패시브)", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                techBotOne = TRUE;
                skillPoint--;
                myMainChaCharacterSkill->TechnicianSkill(1);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"기술자", MB_OK);
            }
        }
        //기술자 스킬 2
        else if (skillNumber == 2) {
            check = MessageBox(g_hWnd, L"체력이 20미만인 상태로 다음 스테이지로 넘어가면 체력 10회복합니다.", L"응급처치 봇 설치(패시브)", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                techBotTwo = TRUE;
                skillPoint--;
                myMainChaCharacterSkill->TechnicianSkill(2);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"기술자", MB_OK);
            }
        }
        //기술자 스킬 3
        else if (skillNumber == 3) {
            check = MessageBox(g_hWnd, L"다음 공격시 적의 방어력을 무시하는 자신의 공격력-2의 추가 공격을 가합니다.", L"마탄의 사수(액티브)", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                skill_1_text = TRUE;
                SetWindowText(skillBtn1, L"마탄의 사수");
                ShowWindow(skillBtn1, SW_SHOW);
                skillPoint--;
                myMainChaCharacterSkill->TechnicianSkill(3);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"기술자", MB_OK);
            }
        }
        //기술자 스킬 4
        else if (skillNumber == 4) {
            check = MessageBox(g_hWnd, L"해당 스테이지에서 받는 피해가 1감소하고\n피해를 받으면 상대방에게 방어력에 영향을 받는 피해량 2를 줍니다.\n중첩", L"리벤저(액티브)", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                skill_2_text = TRUE;
                SetWindowText(skillBtn2, L"리벤저");
                ShowWindow(skillBtn2, SW_SHOW);
                skillPoint--;
                myMainChaCharacterSkill->TechnicianSkill(4);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"기술자", MB_OK);
            }

        }
        //기술자 스킬 5
        else if (skillNumber == 5) {
            check = MessageBox(g_hWnd, L"(주사위 + 설치된 장비 개수) * (공격력-6)의 공격을 가합니다.", L"기계 반란(액티브)", MB_OKCANCEL);
            if (check == IDOK && skillPoint >= 1) {
                skill_3_text = TRUE;
                SetWindowText(skillBtn3, L"기계 반란");
                ShowWindow(skillBtn3, SW_SHOW);
                skillPoint--;
                myMainChaCharacterSkill->TechnicianSkill(5);
            }
            else if (check == IDOK && skillPoint <= 0) {
                MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"기술자", MB_OK);
            }
        }
    }
}

int CriticalHit(int flagNumber) {
    int hit = 0;  // 크리티컬 터졌나?
    int defaultCritical = 5;  // 적 기본 크확
    int critical;  // 크리티컬 확률 변수
    critical = (rand() % 100) + 1;
    if (flagNumber == 1 && critical <= (myMainCharacter->getCritical() + assassinCriticalUp + treasureCritical)) {
        return 1;
    }
    else if (flagNumber == 2 && critical <= defaultCritical) {
        return 2;
    }
    return 0;
}

void AttackButtonClicked(int whoFlag, HWND atkButtonHWND, HDC atkButtonHDC) {
    //whoFlag 1 -> 플레이어 / whoFlag 2 -> 적
    if (whoFlag == 1) {
        flagCritical = 0;
        flagCritical = CriticalHit(1);
        AttackFastSummary(atkButtonHWND);
        HitByCharater(atkButtonHWND, atkButtonHDC);
    }
    else {
        flagCritical = 0;
        flagCritical = CriticalHit(2);
        AttackFastSummary(atkButtonHWND);
        HitByCharater(atkButtonHWND, atkButtonHDC);
    }
    GameUI(atkButtonHWND);
}