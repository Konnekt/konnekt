/** \fn extern "C" int __export __stdcall IMessageProc(unsigned int , int , int , unsigned int);

G³ówna funkcja przetwarzaj¹ca komunikaty (wys³ane przez Controler::IMessage() ).

\param id - identyfikator komunikatu
\param p1 , p2 - dwa parametry typu <b>unsigned long int</b> (rzutowane np. na wskaŸniki do struktur)
\param sender - ID wtyczki wysy³aj¹cej komunikat . 0 - komunikat od rdzenia
\warning IMessageProc \i MUSI obs³ugiwaæ Controler::error i komunikat #IM_PLUG_INIT

IMessageProc jest \i jedyn¹ wymagan¹ funkcj¹ eksportowan¹ z dll'a.
Opisy wszystkich mo¿liwych kodów komunikatów mo¿na znaleŸæ w: #gr_im <br>
Wtyczki bêd¹ otrzymywa³y TYLKO komunikaty o identyfikatorach wiêkszych od #IM_BASE<br><br>
Poni¿szy przyk³ad pokazuje najlepszy sposób zaimplementowania kodu dla IMessageProc().

 \code
   int Init() {
     // inicjalizacja
     return 1;
   }
   int __stdcall IMessageProc(unsigned int id , int p1 , int p2 , unsigned int sender) {
     switch (id) {
        case IM_PLUG_INIT:        // Inicjalizacja wtyczki.
           Ctrl=(Controler*)p1;       // Przyjêcie adresu Controler
           ID=(unsigned int)p2;   // Przyjêcie ID wtyczki
           hDll=(HINSTANCE)ID;    // ID wtyczki jako HINSTANCE ()
                                  //   pod windowsem ID jest w istocie
                                  //   uchwytem do biblioteki .dll
           return Init();
        default:                  // Je¿eli wtyczka nie obs³uguje komunikatu
                                  // KONIECZNIE musi to zaznaczyæ w Ctrl->error!!!
           if (Ctrl) Ctrl->setError(IMERROR_UNSUPPORTEDMSG);
           return 0;
     }
     if (Ctrl) Ctrl->setError(IMERROR_NORESULT);
                                  // Je¿eli wtyczka nie zwraca konkretnego wyniku
                                  // MUSI to zaznaczyæ!
								  // Opcjonalnie wszystkie wtyczki mog¹ zwracaæ
								  // wynik w case , a w tym miejscu mo¿na umieœciæ IMERROR_UNSUPPORTEDMSG
     return 0;
   }
 \endcode
*/

