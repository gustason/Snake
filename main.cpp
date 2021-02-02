#include<iostream>
using namespace std;
void FAST_IO() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
}

bool gameOver;
const int width = 20;
const int height = 20;
int x, y;
void Setup() {
    gameOver = false;
}
void Draw() {

}
void Input() {

}
void Logic() {

}
int main() {
    while(!gameOver) {
        Draw();
        Input();
        Logic();
    }

    return 0;
}
