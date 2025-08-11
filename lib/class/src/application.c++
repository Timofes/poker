#include "../../stdafx.h++"

std::vector<Statistic> application::data = []() {
    std::string file = PATH_TO_DB + "all.txt";
    DataBase A(file);
    return A.getData();
}();

int WINAPI application::WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSA wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "Sample Window Class";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    
    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
        0,                              
        "Sample Window Class",                    
        "Мое приложение",            
        WS_OVERLAPPEDWINDOW,           
        
        CW_USEDEFAULT, CW_USEDEFAULT, widthWindow, heightWindow,
        
        NULL,       
        NULL,      
        hInstance,  
        NULL    
    );

if (!hwnd) {
    MessageBoxW(NULL, L"Ошибка создания окна", L"Ошибка", MB_ICONERROR);
    return 1;
}
    if (hwnd == NULL)
    {
        return 0;
    }
    
    ShowWindow(hwnd, nCmdShow);
    
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}

void application::printAll(HWND hwnd){

    PAINTSTRUCT ps;
    static HDC hdc = BeginPaint(hwnd, &ps);

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmMem = CreateCompatibleBitmap(hdc, widthWindow, heightWindow);
    SelectObject(hdcMem, hbmMem);
    
    Rectangle(hdcMem, 0, 0, widthWindow, heightWindow);

    Rectangle(hdcMem, startXMenu, startYMenu, widthMenu, heightMenu);
    Rectangle(hdcMem, 
        startXStatisticMenu, 
        startYStatisticMenu, 
        widthStaticsticMenu + startXStatisticMenu - marginStatisticMenu, 
        heightStatisticMenu + startYStatisticMenu - marginStatisticMenu);
    printTable(hdcMem);
    
    BitBlt(hdc, 0, 0, widthWindow, heightWindow, hdcMem, 0, 0, SRCCOPY);
    
    DeleteObject(hbmMem);
    DeleteDC(hdcMem);

    EndPaint(hwnd, &ps);

}

inline int application::rangeToRGB(double x){
    return x / ONE_OF_255;
}

void application::printTable(HDC hdc){

    RECT rect{0, 0, 0, 0};
    std::pair<int, int> count;

    for(int i = startXTable; i < startXTable + widthTable; i = i + widthCellTable){
        if(count.second == COUNT_CART){
            break;
        }
        count.second++;
        for(int j = startYTable; j < startYTable + heightTable; j = j + heightCellTable){
            if(count.first == COUNT_CART){
                break;
            }
            count.first++;
            rect.left = i + 1;
            rect.top = j + 1;
            rect.right = i + widthCellTable;
            rect.bottom = j + heightCellTable;

            Statistic sum;
            int iCorrect = (i - startXTable) / widthCellTable;
            int jCorrect = (j - startYTable) / heightCellTable;
            for(int ii = 0; ii < COUNT_SUIT; ii++){
                for(int jj = 0; jj < COUNT_SUIT; jj++){
                    Statistic temp = data[
                        COUNT_SUIT * iCorrect + COUNT_ALL_CART * COUNT_SUIT *
                        jCorrect + ii * COUNT_ALL_CART + jj];
                    sum.win += temp.win;
                    sum.draw += temp.draw;
                    sum.lose += temp.lose;
                }
            }
            double total = sum.draw + sum.lose + sum.win;
            double procW = 0;
            double procL = 0;
            double procD = 0;
            if(total != 0){
                procW = sum.win / total;
                procL = sum.lose / total;
                procD = sum.draw / total;
            }
            int R = rangeToRGB(procL);
            int G = rangeToRGB(procW);
            int B = rangeToRGB(procD);
            HBRUSH hbr = CreateSolidBrush(RGB(R, G, B));
            FillRect(hdc, &rect, hbr);
            DeleteObject(hbr);
        }
        count.first = 0;
    }
}

LRESULT CALLBACK application::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    static HWND hList;

    switch (uMsg)
    {
    case WM_CREATE: {
/*
        #pragma region LISTVIEW
        hList = CreateWindowEx(
            0,
            WC_LISTVIEW, 
            TEXT(""),
            WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SINGLESEL | LVS_NOSORTHEADER | WS_BORDER,
            wightPlateMenu, 
            MARGIN_PX_PLATE_TABLE, 
            wightPlateTable, 
            heightPlateTable,
            hwnd,
            (HMENU)1,
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );

        LVCOLUMN lvc;
        lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

        for(int i = 0; i < COUNT_CART; i++){
            TCHAR szName[32]; 
            _stprintf(szName, _T("%d"), i);
            lvc.iSubItem = 0;
            lvc.pszText = szName;
            lvc.cx = (wightPlateTable / COUNT_CART);
            lvc.fmt = LVCFMT_LEFT;
            ListView_InsertColumn(hList, 0, &lvc);
        }

        LVITEM lvi;
        lvi.mask = LVIF_TEXT;

        TCHAR lviName1[] = TEXT("1");
        TCHAR lviName2[] = TEXT("2");
        TCHAR lviName3[] = TEXT("3");

        for(size_t i = 0; i < COUNT_CART; i++){
            lvi.iItem = i;
            lvi.iSubItem = 0;
            lvi.pszText = lviName1;
            ListView_InsertItem(hList, &lvi);
            for(size_t j = 0; j < COUNT_CART; j++){
                ListView_SetItemText(hList, i, j, lviName3);
            }
        }

        #pragma endregion
*/
    }
        break;
    case WM_PAINT:
    {
        printAll(hwnd);
        return 0;
    }
    case WM_COMMAND:

        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE: {
        //widthWindow = LOWORD(lParam); 
        //heightWindow = HIWORD(lParam);

        //widthCellTable =  widthPlateTable / COUNT_CART;
        //heightCellTable = heightPlateTable / COUNT_CART;
        /*SetWindowPos(hList, NULL, 
            wightPlateMenu, 
            MARGIN_PX_PLATE_TABLE, 
            wightPlateTable, 
            heightPlateTable,
                    SWP_NOZORDER);
        for(size_t i = 0; i < COUNT_CART; i++){
            ListView_SetColumnWidth(hList, i, (wightPlateTable / COUNT_CART));
        }*/
        
        printAll(hwnd);
        break;
    }
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}