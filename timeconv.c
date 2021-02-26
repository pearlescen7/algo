#include <stdio.h>
#include <stdlib.h>

char* timeConversion(char* s) {
    static char hour[8];
    if (s[8] == 'P'){
        hour[0] = s[0];
        hour[1] = s[1];
         int hour_conv = strtol(hour, NULL, 10);
        if (hour_conv == 12){
            hour[0] = '1';
            hour[1] = '2';
        } else{
            hour_conv += 12;
            char temp_hour[12];
            sprintf(temp_hour, "%d", hour_conv);
            hour[0] = temp_hour[0];
            hour[1] = temp_hour[1];
        }
        for(int i = 0; i < 6; i++){
            hour[(i+2)] = s[i+2];
        }
    } else {
        if ((s[0] == '1') && (s[1] == '2')){
            hour[0] = '0';
            hour[1] = '0';
            for(int i = 0; i < 6; i++){
                hour[(i+2)] = s[(i+2)];
            }
        } else{
            for(int i = 0; i < 8; i++){
                    hour[(i)] = s[(i)];
            }
        }
    }
    
    return hour;
}


int main(){
    while (1){
        char s[10];
        char empty[1];
        printf("Enter the hour to convert: ");
        fgets(s, 10, stdin);
        char* result = timeConversion(s);
        printf("%s\nPress enter to quit.", result);
        while (getchar() != '\n'){
            continue;
        }
    }
    return 0;
}