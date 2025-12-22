# 🔍 Picoshell To'liq Tushuntirish - Mutlaq Yangi Boshlovchilar Uchun

Bu hujjat picoshell implementatsiyasining **har bir tafsilotini** tushuntiradi. Hech qanday oldindan bilim talab qilinmaydi!

## 🤔 Picoshell nima?

Tasavvur qiling, siz bir nechta dasturlarni ishga tushirib, ularni bir-biriga bog'lamoqchisiz:
- `ls -l` ni ishga tushiring (fayllarni ro'yxatlash)
- Bu natijani `wc -l` ga yuboring (qatorlarni sanash)
- Bu "pipeline" deyiladi: `ls -l | wc -l`

**Picoshell aynan shuni qiladi!** U dasturlarni bog'laydi, shunda ma'lumotlar biridan ikkinchisiga oqib boradi.

## 📚 Asosiy Tushunchalar (Bu yerdan boshlang!)

### Process (Jarayon) nima?
```
Process ni kompyuteringizdagi "ishchi" deb tasavvur qiling:
- Har bir dastur (ls, wc, grep) alohida ishchi sifatida ishlaydi
- Ishchilar bir-biri bilan to'g'ridan-to'g'ri gaplasha olmaydi
- Ularga ma'lumot almashish uchun maxsus "quvurlar" (pipe) kerak
```

### Pipe (Quvur) nima?
```
Pipe ikki ishchini bog'laydigan quvur kabi:
┌─────────┐    PIPE    ┌─────────┐
│   ls    │ =========> │   wc    │
│(ishchi1)│            │(ishchi2)│
└─────────┘            └─────────┘
```

### File Descriptor (Fayl Deskriptor) nima?
```
Har bir process 3 ta standart "kanal"ga ega:
- 0 = stdin  (klaviatura kirishi)
- 1 = stdout (ekran chiqishi) 
- 2 = stderr (xato xabarlari)

Biz bu kanallarni pipe'larga yo'naltira olamiz!
```

## 🏗️ Kod Strukturasini Tahlil Qilish

### Funksiya Imzosi
```c
int picoshell(char **cmds[])
```

**Bu nimani anglatadi:**
- `cmds` - buyruqlar massivining massivi
- Har bir buyruq ishga tushirishga tayyor (masalan `{"ls", "-l", NULL}`)
- Barcha buyruqlar muvaffaqiyatli bo'lsa 0, birontasi muvaffaqiyatsiz bo'lsa 1 qaytaradi

### Kirish Formatining Misoli
```c
// Ishga tushirish uchun: ls -l | wc -l
char *cmd1[] = {"ls", "-l", NULL};     // Birinchi buyruq
char *cmd2[] = {"wc", "-l", NULL};     // Ikkinchi buyruq  
char **cmds[] = {cmd1, cmd2, NULL};    // Buyruqlar massivi
```

## 🔍 Qator-qator Kod Tushuntirishi

### 1. O'zgaruvchilarni E'lon Qilish
```c
int status;      // Bola jarayonlarning chiqish holatini saqlaydi
int ret = 0;     // Qaytish qiymati (0=muvaffaqiyat, 1=xato)
int fd[2];       // Pipe fayl deskriptorlarini saqlash uchun massiv
int i = 0;       // Buyruqlar uchun sikl hisoblagichi
int num = count_commands(cmds);  // Jami buyruqlar soni
pid_t pid;       // fork() dan keyin jarayon ID sini saqlaydi
int in_fd = 0;   // Keyingi buyruq uchun kirish fayl deskriptorini saqlaydi
```

**Nega bular kerak:**
- `status`: Buyruqlar muvaffaqiyatli yoki muvaffaqiyatsiz ekanligini tekshirish uchun
- `fd[2]`: Har bir pipe 2 ta fayl deskriptor yaratadi (o'qish va yozish)
- `in_fd`: Keyingi buyruq uchun "kirish quvuri"ni eslab qolish uchun

### 2. Asosiy Sikl
```c
while (cmds[i])  // Har bir buyruqni ketma-ket ko'rib chiqish
```

Bu buyruqlarni birin-ketin qayta ishlaydi: cmd1, keyin cmd2, keyin cmd3, va hokazo.

### 3. Pipe'lar Yaratish
```c
if (i < num - 1)    // Agar oxirgi buyruq bo'lmasa
    pipe(fd);       // Yangi pipe yaratish
```

**Nega bu mantiq?**
- N ta buyruq uchun N-1 ta pipe kerak
- Oxirgi buyruqqa chiqish pipe'i kerak emas (ekranga chiqadi)

**`pipe(fd)` nima qiladi:**
```c
// Oldin: fd bo'sh
pipe(fd);
// Keyin: fd[0] = o'qish uchi, fd[1] = yozish uchi

Misol:
fd[0] = 3  (bundan o'qish)
fd[1] = 4  (bunga yozish)
```

### 4. Bola Jarayonlarini Yaratish
```c
pid = fork();
if (!pid)  // Bu kod bola jarayonida ishlaydi
{
    // Bola jarayon kodi shu yerda...
}
```

**`fork()` nima qiladi:**
```
fork() dan oldin:     fork() dan keyin:
┌─────────────┐    ┌─────────────┐  ┌─────────────┐
│   Ota-ona   │    │   Ota-ona   │  │    Bola     │
│             │ => │   pid = 123 │  │   pid = 0   │
└─────────────┘    └─────────────┘  └─────────────┘
```

### 5. Bola Jarayonini Sozlash

#### Chiqishni Sozlash (agar oxirgi buyruq bo'lmasa)
```c
if (i < num - 1)           // Agar oxirgi buyruq bo'lmasa
{
    dup2(fd[1], 1);        // stdout ni pipe yozish uchiga yo'naltirish
    close(fd[0]);          // Ishlatilmayotgan o'qish uchini yopish
    close(fd[1]);          // Asl yozish uchini yopish
}
```

**Vizual tushuntirish:**
```
dup2() dan oldin:
stdout (1) -----> ekran
fd[1] (4) ------> pipe

dup2(fd[1], 1) dan keyin:
stdout (1) -----> pipe  (yo'naltirildi!)
fd[1] (4) ------> pipe  (hali ham bor)

close(fd[1]) dan keyin:
stdout (1) -----> pipe  (yagona bog'lanish qoldi)
```

#### Kirishni Sozlash (agar birinchi buyruq bo'lmasa)
```c
if (i > 0)                 // Agar birinchi buyruq bo'lmasa
{
    dup2(in_fd, 0);        // stdin ni oldingi pipe dan yo'naltirish
    close(in_fd);          // Asl fayl deskriptorni yopish
}
```

**Bu nima qiladi:**
- Oldingi buyruqning chiqishidan kirish oladi
- `in_fd` oldingi iteratsiyadan saqlangan edi

#### Buyruqni Bajarish
```c
execvp(cmds[i][0], cmds[i]);
```

**`execvp()` nima qiladi:**
- Joriy jarayonni yangi dastur bilan almashtiradi
- `cmds[i][0]` - dastur nomi (masalan "ls")
- `cmds[i]` - to'liq argument massivi (masalan {"ls", "-l", NULL})
- Agar muvaffaqiyatli bo'lsa, bu qator hech qachon qaytmaydi!
- Agar muvaffaqiyatsiz bo'lsa, bola jarayon chiqishi kerak

### 6. Ota-ona Jarayonini Tozalash
```c
if (i > 0)
    close(in_fd);          // Oldingi kirish pipe'ini yopish

if (i < num - 1)
{
    in_fd = fd[0];         // Keyingi buyruq uchun o'qish uchini saqlash
    close(fd[1]);          // Yozish uchini yopish (bola uni ishlatmoqda)
}
```

**Nega bu tozalash kerak?**
- Ota-onaga yozish uchi kerak emas (bola uni ishlatadi)
- Keyingi buyruqning kirishi uchun o'qish uchini saqlash
- Oldingi kirishni yopish (endi kerak emas)

### 7. Barcha Bolalarni Kutish
```c
while (wait(&status) > 0)
{
    if (WIFEXITED(status) && WEXITSTATUS(status))
        ret = 1;           // Buyruq muvaffaqiyatsiz
    else if (!WIFEXITED(status))
        ret = 0;           // Buyruq o'ldirildi (hali ham muvaffaqiyat?)
}
```

**Bu nima qiladi:**
- Har bir bola jarayonning tugashini kutadi
- Biron bir buyruq muvaffaqiyatsiz bo'lganini tekshiradi
- Shunga mos ravishda qaytish qiymatini o'rnatadi

## 🌊 To'liq Oqim Misoli: `ls -l | wc -l`

### 1-qadam: Sozlash
```
Buyruqlar: [{"ls", "-l", NULL}, {"wc", "-l", NULL}, NULL]
num = 2 (ikki buyruq)
i = 0 (birinchi buyruqdan boshlanadi)
```

### 2-qadam: Birinchi Buyruq (ls -l)
```
1. i=0, num=2, demak i < num-1 → pipe yaratish
   pipe(fd) → fd[0]=3, fd[1]=4

2. fork() → bola jarayonini yaratadi

3. Bola jarayon (ls):
   - i < num-1 → stdout ni pipe ga yo'naltirish
     dup2(fd[1], 1) → stdout endi pipe ga boradi
     close(fd[0]), close(fd[1]) → tozalash
   - i=0, shuning uchun kirish yo'naltirilishi kerak emas
   - execvp("ls", {"ls", "-l", NULL}) → ls dasturiga aylanadi

4. Ota-ona jarayon:
   - i=0, shuning uchun oldingi in_fd yopish kerak emas
   - i < num-1 → keyingi buyruq uchun o'qish uchini saqlash
     in_fd = fd[0] (keyingi buyruq uchun 3 ni saqlash)
     close(fd[1]) (4 ni yopish, bola uni ishlatmoqda)
   - i++ → keyingi buyruqqa o'tish
```

### 3-qadam: Ikkinchi Buyruq (wc -l)
```
1. i=1, num=2, demak i >= num-1 → yangi pipe kerak emas

2. fork() → ikkinchi bola jarayonini yaratadi

3. Bola jarayon (wc):
   - i >= num-1 → chiqish yo'naltirilishi yo'q (ekranga boradi)
   - i > 0 → kirishni oldingi pipe dan yo'naltirish
     dup2(in_fd, 0) → stdin endi pipe dan keladi (fd 3)
     close(in_fd) → tozalash
   - execvp("wc", {"wc", "-l", NULL}) → wc dasturiga aylanadi

4. Ota-ona jarayon:
   - i > 0 → oldingi kirishni yopish
     close(in_fd) → fd 3 ni yopish
   - i >= num-1 → yangi pipe boshqarish kerak emas
   - i++ → sikl tugaydi (cmds[2] NULL)
```

### 4-qadam: Kutish va Tozalash
```
Ota-ona ikkala bolani kutadi:
- ls tugaydi, holatni qaytaradi
- wc tugaydi, holatni qaytaradi
- Agar ikkalasi ham muvaffaqiyatli → 0 qaytarish
- Agar birontasi muvaffaqiyatsiz → 1 qaytarish
```

## 🎯 Asosiy Tushunchalar

### Nega Bu Ishlaydi
1. **Har bir buyruq o'z jarayonida ishlaydi** - ular bir-biriga xalaqit bera olmaydi
2. **Pipe'lar jarayonlarni bog'laydi** - ma'lumotlar avtomatik oqib boradi
3. **Fayl deskriptor yo'naltirilishi** - dasturlarga oddiy kirish/chiqishga yozayotgandek tuyuladi
4. **Ota-ona hamma narsani muvofiqlashtiradi** - pipe'lar yaratadi, bolalarni fork qiladi, tugashini kutadi

### `dup2()` ning Sehri
```c
dup2(fd[1], 1);  // "stdout (1) ni fd[1] bilan bir xil joyga yo'naltir"
```
Bu chaqiruvdan keyin, dastur stdout ga yozgan har qanday narsa aslida pipe ga boradi!

### Nega Fayl Deskriptorlarni Yopish Kerak?
- **Deadlock oldini olish**: Agar yozish uchi ochiq qolsa, o'quvchi abadiy kutadi
- **Resurs oqishini oldini olish**: Har bir jarayonda cheklangan fayl deskriptorlar bor
- **Toza ajratish**: Har bir jarayon faqat kerak bo'lganini saqlaydi

## 🚨 Keng Tarqalgan Xatolar va Kod Ulardan Qanday Qochadi

### Xato 1: Fayl deskriptorlarni yopmaslik
**Muammo**: Jarayonlar hech qachon kelmaydigan kirishni kutib osilib qoladi
**Yechim**: Kod pipe'larning ishlatilmayotgan uchlarini ehtiyotkorlik bilan yopadi

### Xato 2: Noto'g'ri pipe bog'lanishlari
**Muammo**: Ma'lumotlar noto'g'ri joyga yoki hech qayerga bormaydi
**Yechim**: Kod oldingi pipe'ning o'qish uchini kuzatish uchun `in_fd` dan foydalanadi

### Xato 3: Bolalarni kutmaslik
**Muammo**: Zombie jarayonlar, noto'g'ri qaytish qiymatlari
**Yechim**: Kod barcha bolalarni kutadi va ularning holatini tekshiradi

## 🎓 Siz O'rganganlaringiz

Bu kodni tushungandan keyin, endi siz bilasiz:
- ✅ Jarayonlar pipe'lar orqali qanday muloqot qilishini
- ✅ Fayl deskriptor yo'naltirilishi qanday ishlashini
- ✅ fork() qanday bola jarayonlarini yaratishini
- ✅ execvp() qanday dasturlarni ishga tushirishini
- ✅ Bir nechta jarayonlarni qanday muvofiqlashtirish kerakligini
- ✅ Shell'lar pipeline'larni qanday amalga oshirishini

**Tabriklaymiz!** Siz Unix/Linux tizimlarining asosiy tushunchalaridan birini tushunyapsiz! 🎉