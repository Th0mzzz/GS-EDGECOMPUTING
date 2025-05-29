// Led 1 é o vermelho, o 2 é o amarelo e o 3 o verde
int leds[] = {6, 2, 3};
int trigger = 5;
int echo = 4;
int buzzer = 7;
int valorPot;
float chuvaMm;
// Configurando o LCD
#include <LiquidCrystal_I2C.h>
#define I2C_ADDR 0x27
#define LCD_COLUMNS 20
#define LCD_LINES 4
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);
void setup()
{
    // Definindo Serial
    Serial.begin(9600);
    // Ligando o LCD
    lcd.init();
    lcd.backlight();
    // Criando looping for para definir os LEDS
    for (int i = 0; i < 3; i++)
    {
        pinMode(leds[i], OUTPUT);
    }
    // Definindo o trigger e o echo do sensor ultrasonico
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
}

// Função para desligar todos os leds a partir de um looping for

void desligarLeds()
{
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(leds[i], LOW);
    }
}
// funcao para emitir um som pelo buzzer
void tocarAlerta()
{
    tone(buzzer, 50, 1000);
}
void loop()
{
    // Envio o pulso
    digitalWrite(trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);

    // Calcula o tempo para retornar e a distancia, em centimetros

    long duracao = pulseIn(echo, HIGH);
    float distancia = duracao / 58;
    // Se tiver um metro de distancia do sensor (100cm), está em nivel seguro
    lcd.setCursor(0, 0);
    lcd.print("Nivel: ");
    if (distancia > 100)
    {
        desligarLeds();
        digitalWrite(leds[2], HIGH);
        lcd.setCursor(7, 0);
        lcd.print("Seguro");

        // Se tiver abaixo de 1 metro e acima de meio metro de distancia do sensor (50cm), está em nivel de atenção
    }
    else if (distancia < 100 && distancia >= 50)
    {
        desligarLeds();
        digitalWrite(leds[1], HIGH);
        lcd.setCursor(7, 0);
        lcd.print("Atencao");

        // Se tiver abaixo de meio metro de distancia do sensor (50cm), está em nivel de perigo e chama a função de tocarAlerta
    }
    else if (distancia < 50)
    {
        desligarLeds();
        digitalWrite(leds[0], HIGH);
        lcd.setCursor(7, 0);
        lcd.print("Perigo");
        tocarAlerta();
    }
    // Pego o valor do potenciometro e trago á um valor mais real em mm, simulando um sensor de chuva, fazendo uma regra de 3.
    // o maximo é 50mm e o minimo é 0, aonde até 24mm a chuva é fraca, a partir de 25mm a chuva é forte e toca o alerta.
    valorPot = analogRead(A3);
    chuvaMm = (valorPot / 1023.0) * 50.0;
    lcd.setCursor(0, 1);
    lcd.print("Precipitacao:");
    lcd.print(chuvaMm);
    lcd.print("mm");
    // Colocando chuva como fraca
    if (chuvaMm <= 24)
    {
        lcd.setCursor(0, 3);
        lcd.print("--  Chuva fraca --");
    }
    else
    {
        // Colocando chuva como intensa
        lcd.setCursor(0, 2);
        lcd.print("-- Alerta --");
        lcd.setCursor(0, 3);
        lcd.print("-- Chuva intensa --");
        if (distancia < 100 && distancia >= 50)
        {
            tocarAlerta();
        }
    }
}
