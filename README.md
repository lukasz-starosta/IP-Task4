# IP-Task4
### Do zrobienia jest 2 slow i 2 fast FT in frequency domain, 6 filtrów, wizualizacja oraz przystosowanie aplikacji do tego taska (12 zadań łącznie)
[T2, wszystkie F](http://ics.p.lodz.pl/~tomczyk/available/po_en/fourth.html)
### Podział:
#### Łukasz:
* Przystosowanie aplikacji do taska
#### Piotrek:



### Possible logarithmic visualisation that can be applied to sndft
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            for (int i = 0; i < 3; i++)
            {
                imageCopy(x, y, i) = std::log(std::abs(resultantMatrix[y][x])) > 0 ? (unsigned char) std::log(
                        std::abs(resultantMatrix[y][x])) : (unsigned char) 0;
            }
        }
        cout << endl;
    }
