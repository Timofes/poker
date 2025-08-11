#pragma once

#include "../../stdafx.h++"

class application{
    private:
        inline static double const ONE_OF_255 = 0.0039215686274509803921568627451;

        inline static int widthWindow = 1200;
        inline static int heightWindow = 800;

        inline static int widthMenu = 80;
        inline static int heightMenu = heightWindow;
        inline static int startXMenu = 0;
        inline static int startYMenu = 0;

        inline static int marginStatisticMenu = 30;
        inline static int startXStatisticMenu = widthMenu + startXMenu + marginStatisticMenu;
        inline static int startYStatisticMenu = marginStatisticMenu;
        inline static int widthStaticsticMenu = 200;
        inline static int heightStatisticMenu = heightWindow - (2 * marginStatisticMenu);

        inline static int marginTable = 30;
        inline static int countCellTable = COUNT_CART + 1;
        inline static int widthCellTable =  40;
        inline static int heightCellTable = 40;
        inline static int widthTable = widthCellTable * countCellTable;
        inline static int heightTable = heightCellTable * countCellTable;
        inline static int startXTable = startXStatisticMenu + widthStaticsticMenu + marginTable;
        inline static int startYTable = marginTable;

        static void printTable(HDC hdc);
        static void printAll(HWND hwnd);
        void getTable(string file);
        inline static int rangeToRGB(double x);

        static std::vector<Statistic> data;
    public:
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
};