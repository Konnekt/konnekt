Projekt KONNEKT
===============

Tak, wiem. Piekło zamarzło. Czas wyjąć trupa z szafy...

Oto przed Wami projekt Konnekt w wersji "źródlanej". Repozytoria większości 
wtyczek i niektórych zależności znajdziecie [tutaj](https://github.com/Konnekt). Jeżeli ktoś poczuje się na tyle odważny, żeby to skompilować - niech koniecznie da znać!

Pamiętaj, że Konnekt poza kilkoma wtyczkami i bibliotekami, nigdy nie był
tworzony jako projekt Open Source. Nie było też czasu, żeby go do wymogów OS
dostosować. Pamiętaj też, że jego początki sięgają 2002 roku, kiedy jego autor
był licealistą, a licealiści myślą o innych rzeczach niż sensowna struktura
projektu, czy unit testy... No, przynajmniej ja wtedy nie myślałem.

Od ostatniego razu gdy robiłem cokolwiek przy projekcie minęło kilka lat, 
wybaczcie więc, że nie pamiętam wszystkiego...

# Kompilacja

Ostatnia paczka była kompilowana pod Visual Studio 2005, ale większość wtyczek 
robiona była jeszcze pod Visual C++ Standard.

## Struktura

 - Depends - biblioteki
 - Doc - dokumentacja
 - Output - katalog na pliki w wersji `Debug`. Struktura taka jak katalog
   Konnekta. Wcześniej nazywał się `__output`, w wielu miejscach ta nazwa może
   być nadal w użyciu!
 - Publish - katalog do publikowania (Wcześniej nazywał się `__publish`, w 
   wielu miejscach ta nazwa może być nadal w użyciu!)
   - ___trunk - taka sama struktura jak w Publish, paczki służą do budowania  
     instalatora
     - konnekt.nsi - skrypt NSIS
   - __main - core i ui w wersji Release + pliki pomocnicze
   - _gg - gg w wersji Release
   - _.. pozostałe wtyczki w wersji Release
   - test - katalog Konnekta do którego kopiowane są wersje Release (testsuite.bat)
   - updater - skrypty do budowania plików dla kUpdate
 - Source
   - core
   - ui
   - ... pozostałe wtyczki główne


## Biblioteki zależne

Jako że biblioteki są stare (wersje z 2003-2007 roku), wrzuciłem je na serwer i do repo...

- Kilka mniejszych znajduje się w katalogu /depends/
- [Biblioteki własne](http://www.konnekt.info/sdk-files/lib.zip) - kompilowane ze źródeł
- [Biblioteki dodatkowe](http://www.konnekt.info/sdk-files/)

## Licencja

Kod udostępniony jest na licencji Mozilla (MPL). Jej treść znajdziecie w pliku [LICENSE.txt](LICENSE.txt)

## Disclaimer

This is a legacy project, published mainly for amusement. And partly for those, that are still using the Konnekt IM. It's terribly outdated, and was never designed for open source . Keep this in mind, please.

[![githalytics.com alpha](https://cruel-carlota.pagodabox.com/bf1be781c6854377b94012a36ca23f22 "githalytics.com")](http://githalytics.com/Konnekt/konnekt)