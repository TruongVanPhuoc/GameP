#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

int ts = 54;
Vector2i offset(48, 24);

struct piece
{
    int x, y, cot, hang, k, match, alpha;
    piece() { match = 0; alpha = 255; }
} 

grid[10][10];

void swap(piece p1, piece p2)
{
    std::swap(p1.cot, p2.cot);
    std::swap(p1.hang, p2.hang);

    grid[p1.hang][p1.cot] = p1;
    grid[p2.hang][p2.cot] = p2;
}


int main()
{
    srand(time(0));
    // Kích thước của background là 740 x 480
    RenderWindow app(VideoMode(740, 480), "GameP");
    app.setFramerateLimit(60);

    Texture a1, a2;
    a1.loadFromFile("images/background.png");
    a2.loadFromFile("images/gems.png");

    Sprite background(a1), gems(a2);

    for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 8; j++)
        {
            grid[i][j].k = rand() % 3;
            grid[i][j].cot = j;
            grid[i][j].hang = i;
            grid[i][j].x = j * ts;
            grid[i][j].y = i * ts;
        }

    int x0, y0, x, y; 
    int click = 0; 
    Vector2i pos;
    bool doi = false ;
    bool dichuyen = false;

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();

            if (e.type == Event::MouseButtonPressed)
                if (e.key.code == Mouse::Left)
                {
                    if (!doi && !dichuyen) click++;
                    pos = Mouse::getPosition(app) - offset;
                }
        }

       

        //Quét hàng ngang và hang dọc 
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
            { 
                // Match hàng dọc 
                if (grid[i][j].k == grid[i + 1][j].k)
                    if (grid[i][j].k == grid[i - 1][j].k)
                        for (int n = -1; n <= 1; n++) grid[i + n][j].match++;
                // Match hàng ngang
                if (grid[i][j].k == grid[i][j + 1].k)
                    if (grid[i][j].k == grid[i][j - 1].k)
                        for (int n = -1; n <= 1; n++) grid[i][j + n].match++;
            }
         // Thao tác click

        if (click == 1)
        {
            x0 = pos.x / ts + 1;
            y0 = pos.y / ts + 1;
        }
        if (click == 2)
        {
            x = pos.x / ts + 1;
            y = pos.y / ts + 1;
            if (abs(x - x0) + abs(y - y0) == 1)
            {
                swap(grid[y0][x0], grid[y][x]); 
                doi = 1; 
                click = 0;
            }
            else click = 1;
        }

        //Di chuyển 
        dichuyen = false;
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
            {
                piece& p = grid[i][j];
                int dx, dy;
                for (int n = 0; n < 4; n++)   // tốc độ 4
                {
                    dx = p.x - p.cot * ts;
                    dy = p.y - p.hang * ts;
                    if (dx) p.x -= dx / abs(dx);
                    if (dy) p.y -= dy / abs(dy);
                }
                if (dx || dy) dichuyen = 1;
            }

        //Match Deleting 
        if (!dichuyen)
            for (int i = 1; i <= 8; i++)
                for (int j = 1; j <= 8; j++)
                    if (grid[i][j].match) if (grid[i][j].alpha > 10) 
                        { grid[i][j].alpha -= 10;
                          dichuyen = true; 
                        }
        //diem
        int diem = 0;
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
                diem += grid[i][j].match;
            

        //swap lại khi không match
        if (doi && !dichuyen)
        {
            if (!diem) swap(grid[y0][x0], grid[y][x]); doi = 0;
        }
        
        if (!dichuyen)
        {
            for (int i = 8; i > 0; i--)
                for (int j = 1; j <= 8; j++)
                    if (grid[i][j].match)
                        for (int n = i; n > 0; n--)
                            if (!grid[n][j].match) { swap(grid[n][j], grid[i][j]); break; };

            for (int j = 1; j <= 8; j++)
                for (int i = 8, n = 0; i > 0; i--)
                    if (grid[i][j].match)
                    {
                        grid[i][j].k = rand() % 7;
                        grid[i][j].y = -ts * n++;
                        grid[i][j].match = 0;
                        grid[i][j].alpha = 255;
                    }
        }


      
        app.draw(background);

        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
            {
                piece p = grid[i][j];
                gems.setTextureRect(IntRect(p.k * 49, 0, 49, 49));
                gems.setColor(Color(255, 255, 255, p.alpha));
                gems.setPosition(p.x, p.y);
                gems.move(offset.x - ts, offset.y - ts);
                app.draw(gems);
            }

        app.display();
    }
    return 0;
}
