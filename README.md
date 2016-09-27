# futbolino

## Dependences
 - PAROLA: https://parola.codeplex.com/
 - MD_MAX72xx: http://arduinocode.codeplex.com/ (download http://arduinocode.codeplex.com/downloads/get/1425138)

### library PATCHES
Move the source outside the src folder to the root of each library

### MD_MAX72xx PATCH
```
#define	USE_PAROLA_HW 0
#define	USE_FC16_HW 1
```
