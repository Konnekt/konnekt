/** \fn extern "C" int __export __stdcall IMessageProc(unsigned int , int , int , unsigned int);

G��wna funkcja przetwarzaj�ca komunikaty (wys�ane przez Controler::IMessage() ).

\param id - identyfikator komunikatu
\param p1 , p2 - dwa parametry typu <b>unsigned long int</b> (rzutowane np. na wska�niki do struktur)
\param sender - ID wtyczki wysy�aj�cej komunikat . 0 - komunikat od rdzenia
\warning IMessageProc \i MUSI obs�ugiwa� Controler::error i komunikat #IM_PLUG_INIT

IMessageProc jest \i jedyn� wymagan� funkcj� eksportowan� z dll'a.
Opisy wszystkich mo�liwych kod�w komunikat�w mo�na znale�� w: #gr_im <br>
Wtyczki b�d� otrzymywa�y TYLKO komunikaty o identyfikatorach wi�kszych od #IM_BASE<br><br>
Poni�szy przyk�ad pokazuje najlepszy spos�b zaimplementowania kodu dla IMessageProc().

 \code
   int Init() {
     // inicjalizacja
     return 1;
   }
   int __stdcall IMessageProc(unsigned int id , int p1 , int p2 , unsigned int sender) {
     switch (id) {
        case IM_PLUG_INIT:        // Inicjalizacja wtyczki.
           Ctrl=(Controler*)p1;       // Przyj�cie adresu Controler
           ID=(unsigned int)p2;   // Przyj�cie ID wtyczki
           hDll=(HINSTANCE)ID;    // ID wtyczki jako HINSTANCE ()
                                  //   pod windowsem ID jest w istocie
                                  //   uchwytem do biblioteki .dll
           return Init();
        default:                  // Je�eli wtyczka nie obs�uguje komunikatu
                                  // KONIECZNIE musi to zaznaczy� w Ctrl->error!!!
           if (Ctrl) Ctrl->setError(IMERROR_UNSUPPORTEDMSG);
           return 0;
     }
     if (Ctrl) Ctrl->setError(IMERROR_NORESULT);
                                  // Je�eli wtyczka nie zwraca konkretnego wyniku
                                  // MUSI to zaznaczy�!
								  // Opcjonalnie wszystkie wtyczki mog� zwraca�
								  // wynik w case , a w tym miejscu mo�na umie�ci� IMERROR_UNSUPPORTEDMSG
     return 0;
   }
 \endcode
*/

