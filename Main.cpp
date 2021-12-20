#include <iostream>
#include "src/PicGridSplit.h"

int main()
{
    PicGridSplit* PGS = new PicGridSplit();
    PGS->InitWindow();
    
    PGS->RenderWindow();
    PGS->UpdaterData();
    system("pause");
    return 0;

}