/***************************************************************************\
*       N-PUZZLE 0.2
* Desenvolvido por Abner Cordeiro
* Iniciado em:  19/03/2013 23:17
* corrigido em: 20/09/2014 22:35
\***************************************************************************/

//#define WINVER 0x0410
#define UNICODE

#include <windows.h >
#include <Windowsx.h>
#include <commctrl.h>
#include <stdio.h   >
#include <time.h    >
#include "resource.h"

#define POSX_BOARD 50//50
#define POSY_BOARD 50
HINSTANCE g_hInst;

int iBoard[3][3];

int g_xPosMouse,g_yPosMouse;
int g_iMinutos ,g_iSegundos;

int g_iRetanguloLargura = 60;
int g_retanguloAltura   = 60;

BOOL bJogando = FALSE;

wchar_t g_wcTempo[MAX_PATH];
HBRUSH g_hBrushFundoBoard;
HBRUSH g_hBrushInativoDesbloqueado;
HBRUSH g_hBrushSelecionado;
HBRUSH g_hBrushBloqueado;
HBRUSH g_hFundoPreto;

HFONT g_hFonte1;
HFONT g_hFonte2;

HPEN g_hPen1;
HMENU g_hMenu;

BOOL VerificaPossibilitade(int x,int y)
{
    if(iBoard[x][y + 1] == 0 && y < 2) return TRUE;
    if(iBoard[x][y - 1] == 0 && y > 0) return TRUE;
    if(iBoard[x + 1][y] == 0 && x < 2) return TRUE;
    if(iBoard[x - 1][y] == 0 && x > 0) return TRUE;
    return FALSE;
}

void IniciaBoard()
{
    int x,y,z = 1;

    for(x = 0; x < 3; x++)
        for(y = 0; y < 3; y++)
            iBoard[x][y] = z,z++;

    iBoard[2][2] = 0;
}

BOOL VerificaFimGame(HWND hwnd)
{

    if((iBoard[0][0] == 1 && iBoard[0][1] == 2 && iBoard[0][2] == 3) &&
            (iBoard[1][0] == 4 && iBoard[1][1] == 5 && iBoard[1][2] == 6) &&
            (iBoard[2][0] == 7 && iBoard[2][1] == 8 && iBoard[2][2] == 0))
    {
        KillTimer(hwnd, 1);
        wchar_t wcBuffer[MAX_PATH];
        wsprintf(wcBuffer,L"Voce Ganhou no tempo de %.2d:%.2d minutos...",g_iMinutos ,g_iSegundos);
        MessageBox(NULL,wcBuffer,L"Parabens!!!",MB_OK);

        IniciaBoard();

        bJogando = FALSE;
        return TRUE;
    }
    return FALSE;
}

void MoveBloco(HWND hwnd)
{
    int x,y;

    for(x = 0; x < 3; x++)
    {
        for(y = 0; y < 3; y++)
        {
            int iLeft,iTop,iRight,iBottom;

            iLeft   = (y * g_iRetanguloLargura) + 1;
            iTop    = (x * g_retanguloAltura) + 1 ;
            iRight  = (y * g_iRetanguloLargura) + g_iRetanguloLargura + 1 ;
            iBottom = (x * g_retanguloAltura) + g_retanguloAltura + 1;

            if(( (g_xPosMouse - POSX_BOARD ) > iLeft && (g_xPosMouse - POSX_BOARD) < iRight) &&
                    ( (g_yPosMouse - POSY_BOARD) > iTop  && (g_yPosMouse - POSY_BOARD ) < iBottom))
            {
                if(iBoard[x][y + 1] == 0 && y < 2)
                {
                    iBoard[x][y + 1] = iBoard[x][y],iBoard[x][y] = 0,VerificaFimGame(hwnd);
                }
                if(iBoard[x][y - 1] == 0 && y > 0)
                {
                    iBoard[x][y - 1] = iBoard[x][y],iBoard[x][y] = 0,VerificaFimGame(hwnd);
                }
                if(iBoard[x + 1][y] == 0 && x < 2)
                {
                    iBoard[x + 1][y] = iBoard[x][y],iBoard[x][y] = 0,VerificaFimGame(hwnd);
                }
                if(iBoard[x - 1][y] == 0 && x > 0)
                {
                    iBoard[x - 1][y] = iBoard[x][y],iBoard[x][y] = 0,VerificaFimGame(hwnd);
                }
                return;
            }
        }
    }
}

void MostraBoard(HDC hdc,int pos_x,int pos_y)
{
    register int x;
    register int y;
    register int iBitmapWidth;
    register int iBitmapHeight;
    RECT textRect;
    wchar_t wcBuff[12];

    iBitmapWidth  = (3 * g_iRetanguloLargura) + 4;
    iBitmapHeight = (3 * g_retanguloAltura) + 4;

    // 15,20,360,270

    HDC     memDC = CreateCompatibleDC(hdc);
    HBITMAP memBM = CreateCompatibleBitmap(hdc,iBitmapWidth ,iBitmapHeight);
    SelectObject(memDC,memBM);

    SelectObject(memDC,g_hBrushFundoBoard);

    HGDIOBJ hGDI_Obj = SelectObject(memDC,g_hPen1);

    Rectangle(memDC,1,1,iBitmapWidth,iBitmapHeight);

    SelectObject(memDC,hGDI_Obj);

    SelectObject(memDC,g_hBrushInativoDesbloqueado);

    for(x = 0; x < 3; x++)
    {
        for(y = 0; y < 3; y++)
        {
            int iLeft,iTop,iRight,iBottom;

            iLeft   = (y * g_iRetanguloLargura) + 2;
            iTop    = (x * g_retanguloAltura) + 2;
            iRight  = (y * g_iRetanguloLargura) + g_iRetanguloLargura+ 2;
            iBottom = (x * g_retanguloAltura) + g_retanguloAltura+ 2;

            if(iBoard[x][y] != 0)
            {
                if(bJogando)
                {
                    if(( (g_xPosMouse - pos_x - 17) > iLeft && (g_xPosMouse - pos_x - 17) < iRight) &&
                            ( (g_yPosMouse - pos_y - 22) > iTop  && (g_yPosMouse - pos_y - 22) < iBottom) &&
                            VerificaPossibilitade(x,y))
                        SelectObject(memDC,g_hBrushSelecionado);
                    else if(!VerificaPossibilitade(x,y))
                        SelectObject(memDC,g_hBrushBloqueado);
                    else
                        SelectObject(memDC,g_hBrushInativoDesbloqueado);
                }
                else
                {
                    SelectObject(memDC,g_hBrushInativoDesbloqueado);
                }

                RoundRect(memDC,iLeft,iTop,iRight,iBottom,5,5);

                SetRect(&textRect, iLeft, iTop, iRight,iBottom);

                wsprintf(wcBuff,L"%d",iBoard[x][y]);

                SelectObject(memDC,g_hFonte1);
                SetBkMode(memDC, TRANSPARENT);
                DrawText(memDC,wcBuff,-1,&textRect, DT_CENTER | DT_NOCLIP | DT_VCENTER | DT_SINGLELINE);
            }
        }
    }

    BitBlt(hdc,pos_x,pos_x,iBitmapWidth + 1,iBitmapHeight + 1,memDC,0,0,SRCCOPY);

    DeleteDC    (memDC);
    DeleteObject(memBM);
}

int *GeraTabuleiro(int iQtaNumeros)
{
    register int iPosNum = 0;

    int *Array = (int*)malloc( (sizeof(int) * iQtaNumeros) + 1 );

    while(iPosNum <= iQtaNumeros)
    {
        register int iGen = (rand() % (iQtaNumeros + 1));
        register int iCount = 0;

        for(; (Array[iCount] != iGen && iCount < iPosNum); iCount++);
        if(iCount == iPosNum)
        {
            Array[iPosNum] = iGen;
            iPosNum++;
        }
    }
    return Array;
}

void NovoJogo(HWND hwnd)
{
    g_iMinutos  = g_iSegundos = 0;

    wsprintf(g_wcTempo,L"Tempo: %.2d:%.2d",0,0);

    KillTimer(hwnd, 1);

    int *pArray = GeraTabuleiro(8);
    int x,y,r = 0;
    for(x = 0; x < 3; x++)
    {
        for(y = 0; y < 3; y++)
        {
            iBoard[x][y] = pArray[r];
            r++;
        }
    }
    free(pArray);
    bJogando = TRUE;
    g_iSegundos++;
    SetTimer(hwnd,1,1000,(TIMERPROC) NULL);
}
BOOL CALLBACK DlgAbout(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
        return TRUE;
    case WM_CLOSE:
    {
        DestroyWindow(hwnd);
    }
    return TRUE;
    }
    return FALSE;
}

void ShowGame(HDC hdc )
{
    HDC     memDC = CreateCompatibleDC(hdc);
    HBITMAP memBM = CreateCompatibleBitmap(hdc,340 ,250);
    SelectObject(memDC,memBM);

    HGDIOBJ hGDI_Obj = SelectObject(memDC,g_hFundoPreto);
    Rectangle(memDC,15,20,340,260);
    SelectObject(memDC,hGDI_Obj);

    MostraBoard(memDC,POSX_BOARD - 15,POSY_BOARD - 20);

    hGDI_Obj = SelectObject(memDC,g_hFonte2);

    SetBkMode(memDC, TRANSPARENT);
    COLORREF CorOriginal = SetTextColor(memDC,RGB(255,255,255));
    TextOut(memDC,230,40,g_wcTempo,lstrlenW(g_wcTempo));
    SetTextColor(memDC,CorOriginal);
    SelectObject(memDC,hGDI_Obj);

    BitBlt(hdc,15,20,340,250,memDC,0,0,SRCCOPY);

    DeleteDC(memDC);
    DeleteObject(memBM);
}

BOOL CALLBACK DlgMain(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        g_hBrushFundoBoard          = CreateSolidBrush(RGB(150,150,150));
        g_hBrushInativoDesbloqueado = CreateSolidBrush(RGB(255,255,255));
        g_hBrushSelecionado         = CreateSolidBrush(RGB(100,255,100));
        g_hBrushBloqueado           = CreateSolidBrush(RGB(100,100,255));
        g_hFundoPreto               = CreateSolidBrush(RGB(0,0,0));
        g_hPen1 = CreatePen( PS_SOLID,2,RGB(255,255,255));

        wsprintf(g_wcTempo,L"Tempo: %.2d:%.2d",0,0);
        IniciaBoard();

        g_hFonte1 = CreateFont(35,0,0,0,FW_DONTCARE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                               CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, VARIABLE_PITCH,/*L"Times New Roman"*/L"Stencil");


        g_hFonte2 = CreateFont(15,0,0,0,FW_DONTCARE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                               CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, VARIABLE_PITCH,/*L"Times New Roman"*/L"Stencil");

        SetMenu(hwnd,g_hMenu);
    }
    return TRUE;

    case WM_TIMER:
        switch (wParam)
        {
        case 1:
        {
            wsprintf(g_wcTempo,L"Tempo: %.2d:%.2d",g_iMinutos,g_iSegundos);
            g_iSegundos++;

            if(g_iSegundos >= 60)
            {
                g_iSegundos = 0;
                g_iMinutos++;
            }
            // melhor prevenir...
            if(g_iMinutos >= 60)
            {
                g_iSegundos = 0;
                g_iMinutos = 0;
            }

            HDC hdc = GetDC(hwnd);

            ShowGame( hdc );

            ReleaseDC(hwnd,hdc);
        }
        return 0;
        }
        return TRUE;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hwnd,&ps);

        ShowGame( hdc );

        EndPaint(hwnd,&ps);
    }
    return TRUE;

    case WM_LBUTTONDOWN:
    {
        g_xPosMouse = GET_X_LPARAM(lParam);
        g_yPosMouse = GET_Y_LPARAM(lParam);

        HDC hdc = GetDC(hwnd);

        if(bJogando)
            MoveBloco(hwnd);

        ShowGame( hdc );

        ReleaseDC(hwnd,hdc);
    }
    return TRUE;

    case WM_MOUSEMOVE:
    {
        g_xPosMouse = GET_X_LPARAM(lParam);
        g_yPosMouse = GET_Y_LPARAM(lParam);

        HDC hdc = GetDC(hwnd);

        ShowGame( hdc );

        ReleaseDC(hwnd,hdc);
    }
    return TRUE;

    case WM_CLOSE:
    {
        DeleteObject(g_hBrushFundoBoard);
        DeleteObject(g_hBrushInativoDesbloqueado);
        DeleteObject(g_hBrushSelecionado);
        DeleteObject(g_hBrushBloqueado);
        DeleteObject(g_hFundoPreto);
        DeleteObject(g_hFonte1);
        DeleteObject(g_hFonte2);
        DeleteObject(g_hPen1);
        DestroyMenu(g_hMenu);

        KillTimer(hwnd, 1);

        EndDialog(hwnd, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        case IDM_NOVO_JOGO1:
            NovoJogo(hwnd);
            break;
        case IDM_SAIR1:
            SendMessage(hwnd,WM_CLOSE,0,0);
            break;
        case IDM_SOBRE_O_AUTOR1:
            DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG2), NULL, (DLGPROC)DlgAbout);
            break;
        }
    }
    return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    g_hInst = hInstance;

    InitCommonControls();

    g_hMenu = LoadMenu(hInstance,MAKEINTRESOURCE(IDR_MENU1));

    srand(time(NULL));

    return DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgMain);
}
