#include "mbed.h"

InterruptIn botao(p5);
DigitalOut ledVerm(p6);
DigitalOut ledAz(p7);
PwmOut intensidade(p8);

Timeout timeout;
Ticker ticker;

enum STATE
{
  DOWN, UP, FALL, MIN, RISE, MAX
};

void iniciar();
void Apertar();
void Soltar();
void Transicao();
void Piscar();

int EstadoAtual;
bool EstadoBotao;
float frequencia;

int main() {
    iniciar();
    botao.rise(&Apertar);
    botao.fall(&Soltar);
}

void Transicao(){
    timeout.attach(&Piscar, 0.1);
    if(EstadoBotao){
        switch (EstadoAtual){
            case UP:
                EstadoAtual = RISE;

                printf("RISE\n");

                break;
            case DOWN:
                EstadoAtual = FALL;

                printf("FALL\n");

                break;
            case FALL:
                if(intensidade==0.0){
                    EstadoAtual = MIN;

                    printf("MIN\n");

                }else{
                    intensidade = intensidade - 0.05;

                    printf("intensidade: %.2f\n",intensidade.read());

                }
                break;
            case RISE:
                if(intensidade==1.0){
                    EstadoAtual = MAX;

                    printf("MAX\n");

                }else{
                    intensidade = intensidade + 0.05;

                    printf("intensidade: %.2f\n",intensidade.read());
                    
                }
                break;
            default:
                if (intensidade >= 1.0) {
                    EstadoAtual = MAX;

                    printf("MAX\n");

                } else if (intensidade <= 0.0) {
                        EstadoAtual = MIN;

                        printf("MIN\n");

                }
                break;
        }
    }
}


void Apertar()
{
    printf("Press Button\n");

    EstadoBotao = true;
    ticker.attach(&Transicao,1.0);
    
}

void iniciar(){

    intensidade = 1.0;
    ledAz = 1.0;
    ledVerm = 0.0;
    EstadoAtual = UP;

    printf("UP\n");
}

void Piscar(){

    switch(EstadoAtual){
        case FALL:
            frequencia = 1;
            ledVerm = !ledVerm;
            break;
        case RISE:
            frequencia = 1;
            ledAz = !ledAz;
            break;
        case MIN:
            frequencia = 0.1;
            ledVerm = !ledVerm;
            break;
        case MAX:
            frequencia = 0.1;
            ledAz = !ledAz;
            break;
        default:
            break;
    }

    timeout.attach(&Piscar, frequencia);
}

void Soltar()
{
    printf("Release Button\n");

    EstadoBotao = false;
    ticker.detach();

    if(EstadoAtual == MAX || EstadoAtual == UP || EstadoAtual == FALL){

        if(intensidade > 0.0){

            ledAz = 0.0;
            ledVerm = 1.0;
            EstadoAtual = DOWN;

            printf("DOWN\n");
        }
    }
    else if(EstadoAtual == MIN || EstadoAtual == DOWN || EstadoAtual == RISE){

        if(intensidade < 1.0){

            ledAz = 1.0;
            ledVerm = 0.0;
            EstadoAtual = UP;

            printf("UP\n");

        }
    }
}
