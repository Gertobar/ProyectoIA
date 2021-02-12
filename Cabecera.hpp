# include<iostream> //Entrada y salida de datos, etc.
# include<cstdlib> // C++ standar library
# include<cstring> //Permite trabajar con cadenas de texto
using namespace std;

class CapturaVideo{
    public:
        int k=3;
        int umbral = 255;
        int minH=2;
        int minS=61;
        int minV=143;
        int maxH=101;
        int maxS=152;
        int maxV=242;
     
        void capturarVideo();       
};
