#include <iostream>
#include <curl/curl.h>
#include <string>
#include <sstream>
#include <fstream>


//struktura uzywana do zwrotu danych
//error = 0 <-- dziala
//error = 1 <-- nie dziala
struct tablica{
    std::string przystanek[100];
    int error=0;
    int dlugosc=0;
};

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    output->append(static_cast<char*>(contents), size* nmemb);
    return size * nmemb;
}

  tablica pobierz(std::string url){
  std::string rdata="x";

  CURL* curl = curl_easy_init();
  
  struct curl_slist* headers = NULL;
  headers = curl_slist_append(headers, "Priority: u=0, i");
  headers = curl_slist_append(headers, "Sec-Fetch-User: ?1");
  headers = curl_slist_append(headers, "Sec-Fetch-Site: same-origin");
  headers = curl_slist_append(headers, "Sec-Fetch-Mode: navigate");
  headers = curl_slist_append(headers, "Sec-Fetch-Dest: document");
  headers = curl_slist_append(headers, "Upgrade-Insecure-Requests: 1");
  headers = curl_slist_append(headers, "Cookie: ROZKLADY_JEZYK=PL; ROZKLADY_WIDTH=200; ROZKLADY_WIZYTA=43; ROZKLADY_OSTATNIA=1736800479; ROZKLADY_AB=0; __utma=174679166.434099270.1734802653.1736796329.1736800472.3; __utmz=174679166.1736800472.3.3.utmcsr=google|utmccn=(organic)|utmcmd=organic|utmctr=(not%20provided); __utmc=174679166; __utmb=174679166.1.10.1736800472; __utmt=1");
  headers = curl_slist_append(headers, "Referer: https://rozklady.mpk.krakow.pl/");
  headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate, br, zstd");
  headers = curl_slist_append(headers, "Accept-Language: en-US,en;q=0.5");
  headers = curl_slist_append(headers, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/png,image/svg+xml,*/*;q=0.8");
  headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:131.0) Gecko/20100101 Firefox/131.0");
  headers = curl_slist_append(headers, "Host: rozklady.mpk.krakow.pl");
  headers = curl_slist_append(headers, "GET /?lang=PL&linia=2 HTTP/1.1");

  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);     
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rdata);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);


    //pobranie danych
    CURLcode err = curl_easy_perform(curl);

    //ewentualne zapisanie strony do pliku
    //std::ofstream file("XD.html");
    //file << rdata;
    
    //obrobka danych
    size_t czy_wystepuje = rdata.find("Trasa:   <BR />");

    if(err == CURLE_OK&&czy_wystepuje!=std::string::npos){
        size_t start = rdata.find("Trasa:");
        size_t end = rdata.find("$( function(){");
        size_t koniec = end;
        std::string tekst = rdata.substr(start,end);
        
        tablica przystanki;
        std::string temp;
    
        start = tekst.find("<div style=' text-align: left; font-size: 20px; white-space: nowrap; padding: 10px; border: 1px solid blue; border-radius: 10px; ' >");
        end = tekst.find("</div>");
        temp = tekst.substr(start+133,end);
        end = temp.find("</div>");
        temp = temp.substr(0,end);
        przystanki.przystanek[0]=temp;

        for(int i = 1; i<40;i++){
            start = tekst.find("<div style=' text-align: left; font-size: 20px; white-space: nowrap; padding: 10px; border: 1px solid blue; border-radius: 10px; ' >",start+10);
            end = tekst.find("</div>");
            temp = tekst.substr(start+133,end);
            end = temp.find("</div>");
            temp = temp.substr(0,end);
            przystanki.przystanek[i]=temp;
            if(start>koniec){
                przystanki.dlugosc = i;
		break;
            }
        }

        curl_easy_cleanup(curl);
        przystanki.error = 0;
        return przystanki;
    }else{
        tablica przystanki;
        przystanki.error = 1;
        return przystanki;
    }
}
