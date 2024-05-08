#include <open.mp>

new Text:Reloginho1, Text:Reloginho2, Text:Reloginho3, HoraRealTimer;

public OnFilterScriptInit()
{
    print("\n--------------------------------------");
    print(" [FS] Hora Real");
    print(" www.sampextremeoficial.blogspot.com");
    print("--------------------------------------\n");

    HoraRealTimer = SetTimer("HoraReal", 1000, true);

    //HORA
    Reloginho2 = TextDrawCreate(551.000000, 28.000000, "~w~~h~00:00:00");
    TextDrawBackgroundColour(Reloginho2, 50);
    TextDrawFont(Reloginho2, TEXT_DRAW_FONT_1);
    TextDrawLetterSize(Reloginho2, 0.360000, 1.900000);
    TextDrawColour(Reloginho2, -1);
    TextDrawSetOutline(Reloginho2, 1);
    TextDrawSetProportional(Reloginho2, true);
    TextDrawSetShadow(Reloginho2, 1);
    TextDrawSetSelectable(Reloginho2, false);
    //Data
    Reloginho1 = TextDrawCreate(543.000000, 1.000000, "~w~~h~00/00/0000");
    TextDrawBackgroundColour(Reloginho1, 50);
    TextDrawFont(Reloginho1, TEXT_DRAW_FONT_1);
    TextDrawLetterSize(Reloginho1, 0.360000, 1.900000);
    TextDrawColour(Reloginho1, -1);
    TextDrawSetOutline(Reloginho1, 1);
    TextDrawSetProportional(Reloginho1, true);
    TextDrawSetShadow(Reloginho1, 1);
    TextDrawSetSelectable(Reloginho1, false);
    //Dia Da Semana
    Reloginho3 = TextDrawCreate(563.000000, 19.000000, "~b~~h~~h~sexta-feira");
    TextDrawBackgroundColour(Reloginho3, 0);
    TextDrawFont(Reloginho3, TEXT_DRAW_FONT_1);
    TextDrawLetterSize(Reloginho3, 0.200000, 1.200000);
    TextDrawColour(Reloginho3, -1);
    TextDrawSetOutline(Reloginho3, 1);
    TextDrawSetProportional(Reloginho3, true);
    TextDrawSetShadow(Reloginho3, 1);
    TextDrawSetSelectable(Reloginho3, false);
    return true;
}

public OnFilterScriptExit()
{
    KillTimer(HoraRealTimer);
    return true;
}

public OnPlayerConnect(playerid)
{
    TextDrawShowForPlayer(playerid, Reloginho1);
    TextDrawShowForPlayer(playerid, Reloginho2);
    TextDrawShowForPlayer(playerid, Reloginho3);
    return true;
}

static stock VerSemana(const PT_Dia, const PT_Mes, const PT_Ano)
{
    new PT_A = (14 - PT_Mes) / 12, PT_B = PT_Ano - PT_A, PT_C = (PT_Mes + (12 * PT_A)) - 2, PT_D = (((((PT_Dia + ((31 * PT_C) / 12)) + PT_B) + (PT_B / 4)) - (PT_B / 100)) + (PT_B / 400)), PT_E = PT_D % 7;
    static const PTSemana[7][14] = { "Domingo", "Segunda-Feira", "Terça-Feira", "Quarta-Feira", "Quinta-Feira", "Sexta-Feira", "Sábado" };
    return PTSemana[PT_E];
}

stock VerMes(const PT_Mes)
{
    static const PTMeses[12][10] =
    {
        "Janeiro", "Fevereiro", "Março", "Abril", "Maio", "Junho",
        "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"
    };

    return PTMeses[PT_Mes - 1];
}

forward HoraReal();
public HoraReal()
{
    new str[100];
    new ano, mes, dia, horas, minutos, segundos;
    getdate(ano, mes, dia);
    gettime(horas, minutos, segundos);

    format(str, sizeof str, "~b~~h~~h~%s", VerSemana(dia, mes, ano));
    TextDrawSetString(Reloginho3, str);

    format(str, sizeof(str), "~w~%02d:%02d:%02d", horas, minutos, segundos);
    TextDrawSetString(Reloginho2, str);

    format(str, sizeof str, "~w~%02d %s %d", dia, VerMes(mes), ano);
    TextDrawSetString(Reloginho1, str);
    SetWorldTime(horas + 4);
    SetWeather(horas);
    TextDrawShowForAll(Reloginho1);
    TextDrawShowForAll(Reloginho2);
    TextDrawShowForAll(Reloginho3);
    return true;
}