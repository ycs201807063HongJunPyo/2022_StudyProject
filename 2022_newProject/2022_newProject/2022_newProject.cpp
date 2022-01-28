// 2022_newProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "2022_newProject.h"
#include "myCharacter.h"
#include "enemyCharacter.h"
#include "Engraving.h"
#include "skillJob.h"

#include "Winuser.h"
#include<stdlib.h>
#include<time.h>

#define MAX_LOADSTRING 100

#define IDC_BTN_START 5000  //시작 버튼 ID
#define IDC_BTN_HELP 5001  //도움말 버튼 ID
#define IDC_BTN_EXIT 5002  //종료 버튼 ID

#define IDC_BTN_ATTACK 5003  //공격 버튼 ID
#define IDC_BTN_SKILL1 5004  //스킬1 버튼 ID
#define IDC_BTN_SKILL2 5005  //스킬2 버튼 ID
#define IDC_BTN_SKILL3 5006  //스킬3 버튼 ID

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.


//사용자 변수
RECT myClientRect;  // 사용가능 영역 크기
int gameStarter = 0;  // 게임 시작 유무 확인
int gameStage = 0;  // 게임스테이지
int diceNumber = 1;  // 주사위 숫자
int gameMoney = 100;  // 게임 자본
int enemyRank = 1;  // 적 반복횟수용

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

//사용자 함수
void ResetGameStater(HWND rs_hWnd);
void AttackCharaterAni(HWND hWnd, int flag);
void AttackFastSummary(HWND hWnd);
void SavaStatPoint(HWND statDlghWnd, HWND mainhWnd);
void GameUI(HWND hWnd);
int AttackTurn(int myFast, int enemyFast);
void HitByCharater(HWND hWnd, HDC hdc);
void GetSkill(HWND g_hWnd, int skillNumber);
int CriticalHit(int flagNumber);

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

HWND gameStartBtn, gameHelpBtn, gameExitBtn;
HWND gameHealthStatic, gameStrStatStatic, gameDpStatStatic, gameMoneyStatic;
HWND atkBtn;
HWND skillBtn1, skillBtn2, skillBtn3;

MyCharacter* myMainCharacter = new MyCharacter;
EnemyCharacter* enemyMainCharacter = new EnemyCharacter;
Engraving* engravingStat = new Engraving;
SkillJob* myMainChaCharacterSkill = new SkillJob;

//스탯 다이얼로그
BOOL CALLBACK StatDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    
    switch (iMessage) {
    case WM_INITDIALOG:
        SetDlgItemInt(hDlg, 100, 100, FALSE);
        SetDlgItemInt(hDlg, 150, 150, FALSE);
        //적용된 스텟 보여주기
        SavaStatPoint(hDlg, hWndUi);
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
        SavaStatPoint(hDlg, hWndUi);
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
        return TRUE;
    case WM_COMMAND:
        //창 닫기
        
        if (LOWORD(wParam) == IDCANCEL || LOWORD(wParam) == IDCLOSE)
        {
            EndDialog(hDlg, LOWORD(wParam));
        }
        //직업 선택
        if (LOWORD(wParam) == IDC_BUTTON_KNIGHT_0_0 && mainSkillSet == 0) {
            check = MessageBox(hDlg, L"기사 전직\n기사로 전직하여 방어력이 8증가하며 기사 스킬을 배울수있게됩니다.\n다른 직업 스킬은 배울수없게됩니다.\n\n스킬 포인트가 필요합니다.", L"기사", MB_OKCANCEL);
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
        //기사이면
        if (mainSkillSet == 1) {
            //기사 스킬 0
            if (LOWORD(wParam) == IDC_BUTTON_KNIGHT_1_1) {
                skillCheck = myMainChaCharacterSkill->GetKnightSkill(0);
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
                skillCheck = myMainChaCharacterSkill->GetKnightSkill(1);
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
                skillCheck = myMainChaCharacterSkill->GetKnightSkill(2);
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
                skillCheck = myMainChaCharacterSkill->GetKnightSkill(3);
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
                skillCheck = myMainChaCharacterSkill->GetKnightSkill(4);
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
                skillCheck = myMainChaCharacterSkill->GetKnightSkill(5);
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
        wsprintfW(skillPointText, L"보유중인 스킬 포인트 : %d", skillPoint);
        tempDlghWnd = GetDlgItem(hDlg, IDC_STATIC_SKILLPOINT);
        SetWindowText(tempDlghWnd, skillPointText);
        GameUI(hWndUi);
        break;
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    HDC MemDC;
    
    HBITMAP myBitmap, oldBitmap;
    switch (message)
    {
    case WM_CREATE:
    {
        hWndUi = hWnd;
        SetWindowPos(hWnd, NULL, 500, 300, 1024, 768, 0);  // 게임창 크기 조절
        GetClientRect(hWnd, &myClientRect);  // 조절된 크기 가져오기
        gameStartBtn = CreateWindow(L"button", L"게 임  시 작", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            (myClientRect.left + 50), 150, 150, 100, hWnd, (HMENU)IDC_BTN_START, NULL, NULL);
        gameHelpBtn = CreateWindow(L"button", L"도 움 말", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            (myClientRect.left + 50), 300, 150, 100, hWnd, (HMENU)IDC_BTN_HELP, NULL, NULL);
        gameExitBtn = CreateWindow(L"button", L"종    료", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            (myClientRect.left + 50), 450, 150, 100, hWnd, (HMENU)IDC_BTN_EXIT, NULL, NULL);
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
                MessageBox(hWnd, L"게임을 시작합니다.", L"게임시작", MB_OK);
                gameStarter = 1;
                gameStage++;
                enemyMainCharacter->EnemyUnit(gameStage, enemyRank);
                ShowWindow(gameStartBtn, SW_HIDE);
                ShowWindow(gameHelpBtn, SW_HIDE);
                ShowWindow(gameExitBtn, SW_HIDE);
                atkBtn = CreateWindow(L"button", L"공격", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    (myClientRect.right - 200), 600, 100, 100, hWnd, (HMENU)IDC_BTN_ATTACK, NULL, NULL);
                //스킬 버튼 일단 가려주고 찍으면 보이게하기
                skillBtn1 = CreateWindow(L"button", L"스킬1", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    (myClientRect.right - 850), 600, 120, 100, hWnd, (HMENU)IDC_BTN_SKILL1, NULL, NULL);

                skillBtn2 = CreateWindow(L"button", L"스킬2", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    (myClientRect.right - 700), 600, 120, 100, hWnd, (HMENU)IDC_BTN_SKILL2, NULL, NULL);

                skillBtn3 = CreateWindow(L"button", L"스킬3", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    (myClientRect.right - 550), 600, 120, 100, hWnd, (HMENU)IDC_BTN_SKILL3, NULL, NULL);

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
                warArea.bottom = 510;
                GameUI(hWnd);
                InvalidateRect(hWnd, &warArea, TRUE);
                ReleaseDC(hWnd, hdc);
                break;
            case IDC_BTN_HELP:
                MessageBox(hWnd, L"도움말 출력.", L"도움말", MB_OK);
                break;

            case IDC_BTN_ATTACK: {
                //플레이어 평타
                if (whoTurn == 1) {
                    hdc = GetDC(hWnd);
                    flagCritical = 0;
                    flagCritical = CriticalHit(1);
                    AttackFastSummary(hWnd);
                    HitByCharater(hWnd, hdc);
                    ReleaseDC(hWnd, hdc);
                }
                //적 공격
                if (whoTurn == 2 && gameStarter == 1) {
                    hdc = GetDC(hWnd);
                    flagCritical = 0;
                    flagCritical = CriticalHit(2);
                    AttackFastSummary(hWnd);
                    HitByCharater(hWnd, hdc);
                    ReleaseDC(hWnd, hdc);
                }
                GameUI(hWnd);
            }
                break;
            case IDC_BTN_SKILL1: {
                if (whoTurn == 1) {
                    flagCritical = 0;
                    myTempAtkBuf += 4;
                    whoTurn = AttackTurn(myMainCharacter->getFastAttack(), enemyMainCharacter->getFastAttack());
                    flagTurn = 0;
                }
                //적 공격
                if (whoTurn == 2 && gameStarter == 1) {
                    hdc = GetDC(hWnd);
                    flagCritical = 0;
                    flagCritical = CriticalHit(2);
                    AttackFastSummary(hWnd);
                    HitByCharater(hWnd, hdc);
                    ReleaseDC(hWnd, hdc);
                }
                GameUI(hWnd);
            }
                break;
            case IDC_BTN_SKILL2: {
                if (whoTurn == 1) {
                    hdc = GetDC(hWnd);
                    flagCritical = 0;
                    flagCritical = CriticalHit(1);
                    knightDoubleAttack = TRUE;
                    AttackFastSummary(hWnd);
                    HitByCharater(hWnd, hdc);
                    ReleaseDC(hWnd, hdc);
                }
                //적 공격
                if (whoTurn == 2 && gameStarter == 1) {
                    hdc = GetDC(hWnd);
                    flagCritical = 0;
                    flagCritical = CriticalHit(2);
                    AttackFastSummary(hWnd);
                    HitByCharater(hWnd, hdc);
                    ReleaseDC(hWnd, hdc);
                }
                GameUI(hWnd);
            }
                break;
            case IDC_BTN_SKILL3: {
                if (whoTurn == 1) {
                    hdc = GetDC(hWnd);
                    flagCritical = 0;
                    flagCritical = CriticalHit(1);
                    knightIncisiveAttack = TRUE;
                    AttackFastSummary(hWnd);
                    HitByCharater(hWnd, hdc);
                    ReleaseDC(hWnd, hdc);
                }
                //적 공격
                if (whoTurn == 2 && gameStarter == 1) {
                    hdc = GetDC(hWnd);
                    flagCritical = 0;
                    flagCritical = CriticalHit(2);
                    AttackFastSummary(hWnd);
                    HitByCharater(hWnd, hdc);
                    ReleaseDC(hWnd, hdc);
                }
                GameUI(hWnd);
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
            case IDRESETGAMESTARTER:
                if (gameStarter == 1) {
                    int resetCheck;
                    resetCheck = MessageBox(hWnd, L"환생한다면 지금까지 진행한 스테이지, 스텟, 스킬을 모두 잃게되지만\n(진행한 스테이지/5)*30의 초기 자금을 받고 1스테이지로 돌아갑니다.\n정말 환생하겠습니까?", L"환생", MB_OKCANCEL);
                    if (resetCheck == IDOK) {
                        ResetGameStater(hWnd);
                    }
                }
                break;
            case IDM_EXIT:
            case IDC_BTN_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }

        break;
    case WM_PAINT:
        {
            
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // 차례 알려주기
            /*
            WCHAR whoTurnText[32];
            if (gameStarter == 1 && whoTurn == 1 && drawDice == FALSE) {
                wsprintfW(whoTurnText, L"플레이어 차례입니다.");
                TextOut(hdc, (myClientRect.right / 2 - 150), 250, whoTurnText, lstrlenW(whoTurnText));
            }
            else if (gameStarter == 1 && whoTurn == 2 && drawDice == FALSE) {
                wsprintfW(whoTurnText, L"적 차례입니다.");
                TextOut(hdc, (myClientRect.right / 2 - 150), 250, whoTurnText, lstrlenW(whoTurnText));
            }
            else {
                wsprintfW(whoTurnText, L"");
                TextOut(hdc, (myClientRect.right / 2 - 150), 250, whoTurnText, lstrlenW(whoTurnText));
            }
            */

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
            
            WCHAR str[32];
            //내 캐릭 체력/속공
            wsprintfW(str, L"체력 : %d, 속공 : %d", myMainCharacter->getCurrentHealth(), playCharaterFast);
            TextOut(hdc, myCharacterRect.left, myCharacterRect.top - 15, str, lstrlenW(str));
            //내 캐릭터 기본 보이기
            
            //기사 이미지 보여주기
            if (gameStarter == 1 && mainSkillSet == 1) {
                MemDC = CreateCompatibleDC(hdc);
                myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_KNIGHT1_IMAGE));
                oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
                BitBlt(hdc, myCharacterRect.left, myCharacterRect.top, 100, 150, MemDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                SelectObject(MemDC, oldBitmap);
                DeleteObject(myBitmap);
                DeleteDC(MemDC);
            }
            //기본 이미지
            else if (gameStarter == 1 && mainSkillSet == 0) {
                MemDC = CreateCompatibleDC(hdc);
                myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_NOOBIE_IMAGE));
                oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
                BitBlt(hdc, myCharacterRect.left, myCharacterRect.top, 100, 150, MemDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                SelectObject(MemDC, oldBitmap);
                DeleteObject(myBitmap);
                DeleteDC(MemDC);
            }
            
            wsprintfW(str, L"체력 : %d, 속공 : %d, 공격력 : %d", enemyMainCharacter->getCurrentHealth(), enemyCharaterFast, enemyMainCharacter->getDamage());
            TextOut(hdc, enemyCharacterRect.left, enemyCharacterRect.top - 15, str, lstrlenW(str));
            int enemyModel = gameStage % 5;
            int getSkill = gameStage % 10;
            int check;
            //떠상 물약
            if (gameStarter == 1 && enemyModel == 0) {
                enemyRank++;
                if (getSkill == 0) {
                    check = MessageBox(hWnd, L"신비로운 힘이 당신 주변에 가까워집니다.\n스킬 포인트를 획득했습니다.", L"스킬포인트", MB_OK);
                    if (check == IDOK) {
                        skillPoint++;
                        GameUI(hWnd);
                    }
                }
                else {
                    check = MessageBox(hWnd, L"떠돌이 상인이 물약을 판매한다고합니다.\n구입시 현재 체력을 15회복합니다.(15 Gold)", L"떠돌이 상인!", MB_OKCANCEL);
                    if (check == IDOK) {
                        if (gameMoney >= 15) {
                            gameMoney -= 15;
                            myMainCharacter->setCurrentHealth(myMainCharacter->getCurrentHealth() + 15);
                            if (myMainCharacter->getHealth() < myMainCharacter->getCurrentHealth()) {
                                myMainCharacter->setCurrentHealth(myMainCharacter->getHealth());
                                GameUI(hWnd);
                            }
                        }
                        else {
                            MessageBox(hWnd, L"돈이 부족합니다!", L"떠돌이 상인!", MB_OK);
                        }
                    }
                }
            }
            //적 모형
            else if (gameStarter == 1 && enemyModel == 1 || enemyModel == 3) {
                if (enemyModel == 1) {
                    MemDC = CreateCompatibleDC(hdc);
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_ENEMYTANKER1));
                    oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
                    BitBlt(hdc, enemyCharacterRect.left, enemyCharacterRect.top, 100, 150, MemDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                    SelectObject(MemDC, oldBitmap);
                    DeleteObject(myBitmap);
                    DeleteDC(MemDC);
                }
                else if (enemyModel == 3) {
                    MemDC = CreateCompatibleDC(hdc);
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_ENEMYGUN1));
                    oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
                    BitBlt(hdc, enemyCharacterRect.left, enemyCharacterRect.top, 100, 150, MemDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                    SelectObject(MemDC, oldBitmap);
                    DeleteObject(myBitmap);
                    DeleteDC(MemDC);
                }
                //Rectangle(hdc, enemyCharacterRect.left, enemyCharacterRect.top, enemyCharacterRect.right, enemyCharacterRect.bottom);
            }
            else if (gameStarter == 1 && enemyModel == 2 || enemyModel == 4) {
                if (enemyModel == 2) {
                    MemDC = CreateCompatibleDC(hdc);
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_ENEMYFASTDOG1));
                    oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
                    BitBlt(hdc, enemyCharacterRect.left, enemyCharacterRect.top, 100, 150, MemDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                    SelectObject(MemDC, oldBitmap);
                    DeleteObject(myBitmap);
                    DeleteDC(MemDC);
                }
                else if (enemyModel == 4) {
                    MemDC = CreateCompatibleDC(hdc);
                    myBitmap = LoadBitmap(hInst, MAKEINTATOM(IDB_ENEMYMACHINE));
                    oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
                    BitBlt(hdc, enemyCharacterRect.left, enemyCharacterRect.top, 150, 150, MemDC, 0, 0, SRCCOPY);  //비트맵 그려주기
                    SelectObject(MemDC, oldBitmap);
                    DeleteObject(myBitmap);
                    DeleteDC(MemDC);
                }
            }
            HitByCharater(hWnd, hdc);
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
    switch (message)
    {
    case WM_INITDIALOG:
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
    int skillPoint = 0;
    ///기사(패시브)
    BOOL knightWarDefence = FALSE;  // 기사 전투 태세
    BOOL knightPowerForce = FALSE;  // 기사 위압
    BOOL knightBlackOut = FALSE;  // 기사 무력화
    ///기사(액티브)
    BOOL knightDoubleAttack = FALSE;  //기사 더블 어택
    BOOL knightIncisiveAttack = FALSE;  //기사 예리한 일격

    myMainCharacter->Reset();
    enemyMainCharacter->Reset();
    engravingStat->Reset();
    myMainChaCharacterSkill->Reset();
    //gameMoney + money해준다
    gameMoney = 100 + money;  // 게임 자본

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
        ///기사 액티브 스킬
        //기사 더블 어택
        if (knightDoubleAttack == TRUE) {
            atkDamage = myTempAtkBuf + myMainCharacter->DefaultAttack((diceNumber *2), myMainCharacter->getDamage());
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

        //크리티컬 확인
        if (flagCritical == 1) {
            atkDamage = atkDamage * 2;
        }
        enemyMainCharacter->setCurrentHealth(enemyMainCharacter->ImHit(atkDamage));
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
    //죽으면 스킵되게 하기
    else if (flag == 2 && enemyMainCharacter->getCurrentHealth() > 0) {
        //기사 전투 태세일때
        atkDamage = enemyTempAtkBuf + enemyMainCharacter->DefaultAttack(diceNumber, enemyMainCharacter->getDamage());
        //크리티컬 확인
        if (flagCritical == 2) {
            atkDamage = atkDamage * 2;
        }
        if (knightWarDefence == TRUE) {
            if (atkDamage < myMainCharacter->getDefence()) {
                enemyMainCharacter->setCurrentHealth(enemyMainCharacter->ImHit(5));
            }
        }
        myMainCharacter->setCurrentHealth(myMainCharacter->ImHit(atkDamage));
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

void SavaStatPoint(HWND statDlghWnd, HWND mainhWnd) {
    WCHAR eDlgBuf[32] = { 0, };
    HWND tempDlghWnd;

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
    GameUI(hWndUi);

}

void GameUI(HWND hWnd) {
    WCHAR UIText[128] = { 0, };
    wsprintfW(UIText, L" 현재 체력 / 최대 체력 : %d / %d \n공격력 : %d + (%d)\n방어력 : %d\n돈 : %d"
        , myMainCharacter->getCurrentHealth(), myMainCharacter->getHealth(), myMainCharacter->getDamage(), myTempAtkBuf, myMainCharacter->getDefence(), gameMoney);
    gameHealthStatic = CreateWindowEx(WS_EX_TRANSPARENT, L"static", UIText, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        (myClientRect.left + 20), 20, 300, 70, hWnd, (HMENU)(HMENU)-1, NULL, NULL);
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
        playCharaterFast += myFast;
        enemyCharaterFast += enemyFast;
        if (knightPowerForce == TRUE) {
            enemyCharaterFast -= 3;
        }
        if (knightBlackOut == TRUE) {
            playCharaterFast -= 2;
        }
        
    }
    return winFast;
}

void HitByCharater(HWND hWnd, HDC hdc) {
    //flag 2 -> 플레이어가 맞음
    //flag 1 -> 적이 맞음
    //적 사망
    if (enemyMainCharacter->getCurrentHealth() <= 0) {
        gameStage++;
        enemyMainCharacter->EnemyUnit(gameStage, enemyRank);
        gameMoney = gameMoney + 10 + (enemyRank * 5);
        
        //속공 초기화 + 스테이지 버프 초기화
        enemyCharaterFast = 0;
        playCharaterFast = 0;
        myTempAtkBuf = 0;
        enemyTempAtkBuf = 0;

        // 전체 초기화
        InvalidateRect(hWnd, NULL, TRUE); 
        UpdateWindow(hWnd);

        ReleaseDC(hWnd, hdc);
    }
    //플레이어 사망
    if (myMainCharacter->getCurrentHealth() <= 0) {
        int resetCheck;
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
        }
        else if (check == IDOK && skillPoint <= 0) {
            MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"기사", MB_OK);
        }
    }
    //기사 스킬 3
    else if (skillNumber == 3) {
        check = MessageBox(g_hWnd, L"해당 스테이지에서 자신의 공격력이 4증가합니다.", L"신의 가호(액티브)", MB_OKCANCEL);
        if (check == IDOK && skillPoint >= 1) {
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
            SetWindowText(skillBtn3, L"예리한 일격");
            ShowWindow(skillBtn3, SW_SHOW);
            skillPoint--;
            myMainChaCharacterSkill->KnightSkill(5);
        }
        else if (check == IDOK && skillPoint <= 0) {
            MessageBox(g_hWnd, L"스킬포인트가 필요합니다!", L"기사", MB_OK);
        }
    }
    
    //나중에 스킬 포인트 제대로 어찌할지 정하고 하기
    //복붙용이니 나머지는 지움
    /*
    if (gameMoney >= engravingStat->costArray[0]) {
        gameMoney -= engravingStat->costArray[0];
        engravingStat->UpStrPoint();
        myMainCharacter->setDamage(1);
    }
    else {
        MessageBox(hDlg, L"돈이 부족합니다!\n스테이지를 진행하며 돈을 획득해주세요", L"더 줘", MB_OK);
    }
    */
}

int CriticalHit(int flagNumber) {
    int hit = 0;  // 크리티컬 터졌나?
    int defaultCritical = 10;  // 기본 크확
    int critical;  // 크리티컬 확률 변수
    srand((unsigned int)time(NULL));
    critical = (rand() % 100) + 1;
    if (critical <= defaultCritical) {
        if (flagNumber == 1) {
            return 1;
        }
        else if (flagNumber == 2) {
            return 2;
        }
    }
    return 0;
}
