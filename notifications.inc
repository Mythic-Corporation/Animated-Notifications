/*

  __  __       _   _     _         _____                                 _   _             
 |  \/  |     | | | |   (_)       / ____|                               | | (_)            
 | \  / |_   _| |_| |__  _  ___  | |     ___  _ __ _ __   ___  _ __ __ _| |_ _  ___  _ __  
 | |\/| | | | | __| '_ \| |/ __| | |    / _ \| '__| '_ \ / _ \| '__/ _` | __| |/ _ \| '_ \ 
 | |  | | |_| | |_| | | | | (__  | |___| (_) | |  | |_) | (_) | | | (_| | |_| | (_) | | | |
 |_|  |_|\__, |\__|_| |_|_|\___|  \_____\___/|_|  | .__/ \___/|_|  \__,_|\__|_|\___/|_| |_|
          __/ |                                   | |                                      
         |___/                                    |_|                                      

    * Este es un pequeño sistema de "notificaciones" con una animación bastante simple para
    darle un poco de vida al servidor en el que se use, es muy común ver este mismo textdraw
    para hacer pequeñas notificaciones al jugador que no sean por chat para que se vea "más moderno"
    así que decidi mejorar un poco el sistema añadiendo una pequeña animación que en versiones futuras
    mejoraré.

    * Nota: Este include tiene dependencia al include-plugin "SKY", si no lo tienen en su carpeta "Pawn/Includes"
    no funcionará correctamente y lanzará errores, espero les sirva el include.

    * Atentamente: Sen Ukiyo - C.E.O Mythic Corporation.


*/


#include <a_samp>
#include <SKY>

// * Defines

//#define ANT_DEBUG; // Modo Debug, ayudará para saber hasta donde se ejecuta el codigo si hay algún error.

#define ANT_FINAL_POSX      320.000000      // Si van a modiciar este número intenten que no tenga decimales.
#define ANT_FINAL_POSY      415.000000      // Si van a modiciar este número intenten que no tenga decimales.
#define ANT_INIT_POSX       320.000000      // Si van a modiciar este número intenten que no tenga decimales.
#define ANT_INIT_POSY       446.000000      // Si van a modiciar este número intenten que no tenga decimales.

#define ANT_TIMEMOVE 5 
// Entre menos sea el número de "ANT_TIMEMOVE" mayor fluides tendrá el movimiento pero tambien se moverá más rápido
// Recomendación: Dejarlo entre 10 y 5.

#define ANT_HOOK%0(%1)              forward %0(%1); public %0(%1)

// * News

new 
    ANT_SHOWTIME[MAX_PLAYERS],
    ANT_TIMER[MAX_PLAYERS],
    PlayerText:NotificationTD[MAX_PLAYERS],
    Float:ANT_ACTUAL_POSY[MAX_PLAYERS] = 446.000000;

// * Callbacks

public OnPlayerConnect(playerid)
{
    NotificationTD[playerid] = CreatePlayerTextDraw(playerid, ANT_INIT_POSX, ANT_INIT_POSY, "_");
    PlayerTextDrawLetterSize(playerid, NotificationTD[playerid], 0.320999, 1.736888);
    PlayerTextDrawAlignment(playerid, NotificationTD[playerid], 2);
    PlayerTextDrawColor(playerid, NotificationTD[playerid], -1);
    PlayerTextDrawSetShadow(playerid, NotificationTD[playerid], 1);
    PlayerTextDrawSetOutline(playerid, NotificationTD[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, NotificationTD[playerid], 255);
    PlayerTextDrawFont(playerid, NotificationTD[playerid], 1);
    PlayerTextDrawSetProportional(playerid, NotificationTD[playerid], 1);
    PlayerTextDrawSetShadow(playerid, NotificationTD[playerid], 1);
    #if defined ANT_DEBUG
        printf("Notification Created p#%i", playerid);
    #endif

    #if defined ANOT_OnPlayerConnect
        return ANOT_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect ANOT_OnPlayerConnect
#if defined ANOT_OnPlayerConnect
    forward ANOT_OnPlayerConnect(playerid);
#endif

// * Function

stock ShowNotification(playerid, const text[], time = 3)
{
    PlayerTextDrawSetString(playerid, NotificationTD[playerid], text);
    PlayerTextDrawShow(playerid, NotificationTD[playerid]);
    ANT_TIMER[playerid] = SetTimerEx("ShowNotificationTextDraw", ANT_TIMEMOVE, true, "i", playerid);
    ANT_SHOWTIME[playerid] = time * 1000;
    #if defined ANT_DEBUG
        printf("ShowNotification p#%i", playerid);
        new string[640];
        format(string, sizeof(string), "[DEBUG] ShowNotification p#%i", playerid);
        SendClientMessage(playerid, -1, string);
    #endif
    return 1;
}

// * Timers

ANT_HOOK ShowNotificationTextDraw(playerid)
{
    if(ANT_ACTUAL_POSY[playerid] == ANT_FINAL_POSY)
    {
        KillTimer(ANT_TIMER[playerid]);
        SetTimerEx("NotificationHideCooldown", ANT_SHOWTIME[playerid], false, "i", playerid);
        #if defined ANT_DEBUG
            printf("KillTimerNotification p#%i", playerid);
            new string[640];
            format(string, sizeof(string), "[DEBUG] KillTimerNotification p#%i", playerid);
            SendClientMessage(playerid, -1, string);
        #endif
        return 1;
    }
    if(ANT_ACTUAL_POSY[playerid] != ANT_FINAL_POSY || ANT_ACTUAL_POSY[playerid] > ANT_FINAL_POSY)
    {
        ANT_ACTUAL_POSY[playerid] -= 0.500000;
        PlayerTextDrawHide(playerid, NotificationTD[playerid]);
        PlayerTextDrawSetPosition(playerid, NotificationTD[playerid], ANT_FINAL_POSX, ANT_ACTUAL_POSY[playerid]);
        PlayerTextDrawShow(playerid, NotificationTD[playerid]);
        #if defined ANT_DEBUG
            printf("MoveNotification Pos %f | PosF %f | p#%i", ANT_ACTUAL_POSY[playerid], ANT_FINAL_POSY, playerid);
            new string[640];
            format(string, sizeof(string), "[DEBUG] MoveNotification Pos %f | PosF %f | p#%i", ANT_ACTUAL_POSY[playerid], ANT_FINAL_POSY, playerid);
            SendClientMessage(playerid, -1, string);
        #endif
        return 1;
    }
    return 1;
}

ANT_HOOK NotificationHideCooldown(playerid)
{
    ANT_TIMER[playerid] = SetTimerEx("HideNotificationTextDraw", ANT_TIMEMOVE, true, "i", playerid);
    #if defined ANT_DEBUG
        printf("CooldownNotification p#%i", playerid);
        new string[640];
        format(string, sizeof(string), "[DEBUG] CooldownNotification p#%i", playerid);
        SendClientMessage(playerid, -1, string);
    #endif
    return 1;
}

ANT_HOOK HideNotificationTextDraw(playerid)
{
    if(ANT_ACTUAL_POSY[playerid] != ANT_INIT_POSY || ANT_ACTUAL_POSY[playerid] < ANT_INIT_POSY)
    {
        ANT_ACTUAL_POSY[playerid] += 0.500;
        PlayerTextDrawHide(playerid, NotificationTD[playerid]);
        PlayerTextDrawSetPosition(playerid, NotificationTD[playerid], ANT_FINAL_POSX, ANT_ACTUAL_POSY[playerid]);
        PlayerTextDrawShow(playerid, NotificationTD[playerid]);
        #if defined ANT_DEBUG
            printf("HideMoveNotification Pos %f | PosF %f | p#%i", ANT_ACTUAL_POSY[playerid], ANT_FINAL_POSY, playerid);
            new string[640];
            format(string, sizeof(string), "[DEBUG] HideMoveNotification Pos %f | PosF %f | p#%i", ANT_ACTUAL_POSY[playerid], ANT_FINAL_POSY, playerid);
            SendClientMessage(playerid, -1, string);
        #endif
    }
    else
    {
        PlayerTextDrawHide(playerid, NotificationTD[playerid]);
        KillTimer(ANT_TIMER[playerid]);
        #if defined ANT_DEBUG
            printf("KillTimerMoveHideNotification p#%i", playerid);
            new string[640];
            format(string, sizeof(string), "[DEBUG] KillTimerMoveHideNotification p#%i", playerid);
            SendClientMessage(playerid, -1, string);
        #endif
    }
    return 1;
}