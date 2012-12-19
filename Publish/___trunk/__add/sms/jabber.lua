--- Skrypt obs³uguj±cy bramkê www->JABBER
--- by Olórin
--- sendSMS jest funkcj± wywo³ywan± z zewn±trz...
--- SMS jest tablic± zawieraj±c± klucze:
--- SMS.msg
--- SMS.to
--- SMS.fromNr
--- SMS.fromName
--- SMS.fromEmail
--- SMS.nazwy_dodatkowych_parametrow

function sendSMS(SMS)
  --- Usuwany zbêdn± (a wrêcz szkodliw±) domenê
		SMS.to = string.gsub(SMS.to, "jabber:", "");
		SMS.to = string.gsub(SMS.to, "jid:", "");			
		SMS.to = string.gsub(SMS.to, "jab:", "");
		SMS.to = string.gsub(SMS.to, "wpk:", "");
		SMS.to = string.gsub(SMS.to, "@wp.pl", "@jabber.wp.pl");
		y=SMS.to
		SMS.to = string.gsub(SMS.to, "@","&serwer=");
		SMS.msg=cp2iso(SMS.msg);	
	--- Funkcje do obs³ugi sieci s± okrojonymi odpowiednikami z WinInet API
	succ=0;
	setInfo("£¹czê...");
	--- Tworzymy sesje internetowa. Zapisujemy uchwyt sesji..
	--- InternetOpen(user_agent);
if SMS.jid == "" or SMS.pass == "" or SMS.serwer == "" or SMS.port == "" then
	alert("Aby wys³aæ wiadomoœæ do u¿ytkownika Jabber'a musisz podaæ dane swojego konta jabber lub skorzystaæ z bramki niewymagaj¹cej konta!!!");
setError("Niepoda³eœ login'u,has³a lub serwera jabbera")
else
	
	hSession = InternetOpen("Mozilla/4.0 (compatible; MSIE 6.0; Windows 98; MyIE2)");
	if hSession ~= 0 then  --- Sprawdzamy czy siê uda³o

		--- £±czymy siê z serwerem i zapisujemy uchwyt po³±czenia.
		--- InternetConnect(sesja , host_serwera , port_serwera , login , haslo);

		hConnect = InternetConnect(hSession, "jab.moon.net.pl", 80, "", "");
		if hConnect ~= 0 then    --- Sprawdzamy czy siê uda³o
			setInfo("Wysy³am zapytanie");
		
						--- Teraz ju¿ wysy³amy

			hRequest = HttpOpenRequest(hConnect, "POST", "index.php" , "");
			if hRequest ~= 0 then   --- sprawdzamy czy siê uda³o
			
				--- Wysy³amy zapytanie do serwera i sprawdzamy czy siê powiod³o.
				--- HttpSendRequest(zapytanie , nag³ówek , dane_dla_post)
				
				data = "j_user=" .. SMS.jid .. "&j_pass=" .. SMS.pass .. "&j_server=" .. SMS.serwer .. "&j_port=" .. SMS.port .. "&adresat=" .. SMS.to .. "&tresc=" .. SMS.msg .. "&op=bramkasenduser";

				
				if HttpSendRequest(hRequest, "Content-Type: application/x-www-form-urlencoded\nReferer: http://konnekt.stamina.eu.org", data) then
				
					--- Wczytujemy wszystko co zwróci³ nam serwer.
					--- InternetReadFile(zapytanie)
					tab={};
					tab[1]="Komunikat na wypadek gdyby nie odnaleziono w³aœciwiego otoczenie komunikatów ?!";
					txt = InternetReadFile(hRequest);
					txt = txt .. "</TD>";
					preg_match("|<B>Bramka WWW -> Jabber</B>(.+?)</TD>|si" , txt , tab);
					res = tab[1];
					---Tutej okreœlamy jak wyglada komunikat potwierdzaj¹cy sukces (znaki specjalne i pl zastêpujemy kropk¹): 
					isok = preg_match("/zosta.a wys.ana./is" , res); 
					res = string.gsub(res, "\n" , "" );
					res = string.gsub(res, "<BR>" , "" );
					res = string.gsub(res, "<B>" , "" );
					res = string.gsub(res, "</b>" , "" );

					if isok <= 0 then 
					---Tu przesy³any jest do wtyczki SMS komunikat o b³êdzie z okreœlonego wczeœniej miejsca strony: 
					setError("B³¹d: " .. res); 
					else 
					---W przeciwnym wypadku komunikato o powodzeniu :) z tego samego miejsca: 
					setSuccess(res); 
					end 

					
					succ = 1;
				end
			end
		end
	end
	if succ == 0 then setError("Wyst±pi³y problemy z po³±czeniem");end;
	if succ == -1 then setError("Wyst±pi³y problemy z bramk±");end;
	--- Zwalniamy uchwyty
	InternetCloseHandle(hSession);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hRequest);
end
end;