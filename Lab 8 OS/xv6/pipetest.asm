
_pipetest:     file format elf32-i386


Disassembly of section .text:

00000000 <main>:
#include "user.h"

#define NULL 0

int main(void)
{
   0:	f3 0f 1e fb          	endbr32 
   4:	8d 4c 24 04          	lea    0x4(%esp),%ecx
   8:	83 e4 f0             	and    $0xfffffff0,%esp
   b:	ff 71 fc             	pushl  -0x4(%ecx)
   e:	55                   	push   %ebp
   f:	89 e5                	mov    %esp,%ebp
  11:	56                   	push   %esi
  12:	53                   	push   %ebx
    char buf[512];
    int n;

    int pr[2], pw[2];
    pipe(pr);
  13:	8d 85 d8 fd ff ff    	lea    -0x228(%ebp),%eax
{
  19:	51                   	push   %ecx
  1a:	81 ec 28 02 00 00    	sub    $0x228,%esp
    pipe(pr);
  20:	50                   	push   %eax
  21:	e8 cd 03 00 00       	call   3f3 <pipe>
    pipe(pw);
  26:	8d 85 e0 fd ff ff    	lea    -0x220(%ebp),%eax
  2c:	89 04 24             	mov    %eax,(%esp)
  2f:	e8 bf 03 00 00       	call   3f3 <pipe>

    int stdout = dup(0);
  34:	c7 04 24 00 00 00 00 	movl   $0x0,(%esp)
  3b:	e8 1b 04 00 00       	call   45b <dup>
  40:	89 c6                	mov    %eax,%esi

    if (!fork())
  42:	e8 94 03 00 00       	call   3db <fork>
  47:	83 c4 10             	add    $0x10,%esp
  4a:	85 c0                	test   %eax,%eax
  4c:	0f 85 8d 00 00 00    	jne    df <main+0xdf>
    {
        close(0); // stdin
  52:	83 ec 0c             	sub    $0xc,%esp
  55:	6a 00                	push   $0x0
  57:	e8 af 03 00 00       	call   40b <close>
        dup(pr[0]);
  5c:	5e                   	pop    %esi
  5d:	ff b5 d8 fd ff ff    	pushl  -0x228(%ebp)
  63:	e8 f3 03 00 00       	call   45b <dup>
        close(1); // stdout
  68:	c7 04 24 01 00 00 00 	movl   $0x1,(%esp)
  6f:	e8 97 03 00 00       	call   40b <close>
        dup(pw[1]);
  74:	58                   	pop    %eax
  75:	ff b5 e4 fd ff ff    	pushl  -0x21c(%ebp)
  7b:	e8 db 03 00 00       	call   45b <dup>

        close(pr[0]);
  80:	58                   	pop    %eax
  81:	ff b5 d8 fd ff ff    	pushl  -0x228(%ebp)
  87:	e8 7f 03 00 00       	call   40b <close>
        close(pr[1]);
  8c:	58                   	pop    %eax
  8d:	ff b5 dc fd ff ff    	pushl  -0x224(%ebp)
  93:	e8 73 03 00 00       	call   40b <close>
        close(pw[0]);
  98:	58                   	pop    %eax
  99:	ff b5 e0 fd ff ff    	pushl  -0x220(%ebp)
  9f:	e8 67 03 00 00       	call   40b <close>
        close(pw[1]);
  a4:	58                   	pop    %eax
  a5:	ff b5 e4 fd ff ff    	pushl  -0x21c(%ebp)
  ab:	e8 5b 03 00 00       	call   40b <close>

        char *argv[] = {"wc", NULL};
        exec("wc", argv);
  b0:	8d 85 e8 fd ff ff    	lea    -0x218(%ebp),%eax
  b6:	5a                   	pop    %edx
  b7:	59                   	pop    %ecx
  b8:	50                   	push   %eax
  b9:	68 a8 08 00 00       	push   $0x8a8
        char *argv[] = {"wc", NULL};
  be:	c7 85 e8 fd ff ff a8 	movl   $0x8a8,-0x218(%ebp)
  c5:	08 00 00 
  c8:	c7 85 ec fd ff ff 00 	movl   $0x0,-0x214(%ebp)
  cf:	00 00 00 
        exec("wc", argv);
  d2:	e8 44 03 00 00       	call   41b <exec>
  d7:	83 c4 10             	add    $0x10,%esp
            printf(stdout, buf);

        close(pw[0]);
    }

    exit();
  da:	e8 04 03 00 00       	call   3e3 <exit>
        close(pr[0]);
  df:	83 ec 0c             	sub    $0xc,%esp
  e2:	ff b5 d8 fd ff ff    	pushl  -0x228(%ebp)
  e8:	e8 1e 03 00 00       	call   40b <close>
        write(pr[1], "hello world\n", 12);
  ed:	83 c4 0c             	add    $0xc,%esp
  f0:	6a 0c                	push   $0xc
  f2:	68 ab 08 00 00       	push   $0x8ab
  f7:	ff b5 dc fd ff ff    	pushl  -0x224(%ebp)
  fd:	e8 01 03 00 00       	call   403 <write>
        close(pr[1]);
 102:	58                   	pop    %eax
 103:	ff b5 dc fd ff ff    	pushl  -0x224(%ebp)
 109:	e8 fd 02 00 00       	call   40b <close>
        wait();
 10e:	e8 d8 02 00 00       	call   3eb <wait>
        printf(stdout, "recieved:\n");
 113:	5a                   	pop    %edx
 114:	59                   	pop    %ecx
 115:	68 b8 08 00 00       	push   $0x8b8
 11a:	56                   	push   %esi
 11b:	e8 20 04 00 00       	call   540 <printf>
        close(pw[1]);
 120:	5b                   	pop    %ebx
 121:	ff b5 e4 fd ff ff    	pushl  -0x21c(%ebp)
 127:	8d 9d e8 fd ff ff    	lea    -0x218(%ebp),%ebx
 12d:	e8 d9 02 00 00       	call   40b <close>
        while ((n = read(pw[0], buf, sizeof(buf))) > 0)
 132:	83 c4 10             	add    $0x10,%esp
 135:	eb 16                	jmp    14d <main+0x14d>
 137:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 13e:	66 90                	xchg   %ax,%ax
            printf(stdout, buf);
 140:	83 ec 08             	sub    $0x8,%esp
 143:	53                   	push   %ebx
 144:	56                   	push   %esi
 145:	e8 f6 03 00 00       	call   540 <printf>
 14a:	83 c4 10             	add    $0x10,%esp
        while ((n = read(pw[0], buf, sizeof(buf))) > 0)
 14d:	83 ec 04             	sub    $0x4,%esp
 150:	68 00 02 00 00       	push   $0x200
 155:	53                   	push   %ebx
 156:	ff b5 e0 fd ff ff    	pushl  -0x220(%ebp)
 15c:	e8 9a 02 00 00       	call   3fb <read>
 161:	83 c4 10             	add    $0x10,%esp
 164:	85 c0                	test   %eax,%eax
 166:	7f d8                	jg     140 <main+0x140>
        close(pw[0]);
 168:	83 ec 0c             	sub    $0xc,%esp
 16b:	ff b5 e0 fd ff ff    	pushl  -0x220(%ebp)
 171:	e8 95 02 00 00       	call   40b <close>
 176:	83 c4 10             	add    $0x10,%esp
 179:	e9 5c ff ff ff       	jmp    da <main+0xda>
 17e:	66 90                	xchg   %ax,%ax

00000180 <strcpy>:
#include "user.h"
#include "x86.h"

char*
strcpy(char *s, const char *t)
{
 180:	f3 0f 1e fb          	endbr32 
 184:	55                   	push   %ebp
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
 185:	31 c0                	xor    %eax,%eax
{
 187:	89 e5                	mov    %esp,%ebp
 189:	53                   	push   %ebx
 18a:	8b 4d 08             	mov    0x8(%ebp),%ecx
 18d:	8b 5d 0c             	mov    0xc(%ebp),%ebx
  while((*s++ = *t++) != 0)
 190:	0f b6 14 03          	movzbl (%ebx,%eax,1),%edx
 194:	88 14 01             	mov    %dl,(%ecx,%eax,1)
 197:	83 c0 01             	add    $0x1,%eax
 19a:	84 d2                	test   %dl,%dl
 19c:	75 f2                	jne    190 <strcpy+0x10>
    ;
  return os;
}
 19e:	89 c8                	mov    %ecx,%eax
 1a0:	5b                   	pop    %ebx
 1a1:	5d                   	pop    %ebp
 1a2:	c3                   	ret    
 1a3:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 1aa:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi

000001b0 <strcmp>:

int
strcmp(const char *p, const char *q)
{
 1b0:	f3 0f 1e fb          	endbr32 
 1b4:	55                   	push   %ebp
 1b5:	89 e5                	mov    %esp,%ebp
 1b7:	53                   	push   %ebx
 1b8:	8b 4d 08             	mov    0x8(%ebp),%ecx
 1bb:	8b 55 0c             	mov    0xc(%ebp),%edx
  while(*p && *p == *q)
 1be:	0f b6 01             	movzbl (%ecx),%eax
 1c1:	0f b6 1a             	movzbl (%edx),%ebx
 1c4:	84 c0                	test   %al,%al
 1c6:	75 19                	jne    1e1 <strcmp+0x31>
 1c8:	eb 26                	jmp    1f0 <strcmp+0x40>
 1ca:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 1d0:	0f b6 41 01          	movzbl 0x1(%ecx),%eax
    p++, q++;
 1d4:	83 c1 01             	add    $0x1,%ecx
 1d7:	83 c2 01             	add    $0x1,%edx
  while(*p && *p == *q)
 1da:	0f b6 1a             	movzbl (%edx),%ebx
 1dd:	84 c0                	test   %al,%al
 1df:	74 0f                	je     1f0 <strcmp+0x40>
 1e1:	38 d8                	cmp    %bl,%al
 1e3:	74 eb                	je     1d0 <strcmp+0x20>
  return (uchar)*p - (uchar)*q;
 1e5:	29 d8                	sub    %ebx,%eax
}
 1e7:	5b                   	pop    %ebx
 1e8:	5d                   	pop    %ebp
 1e9:	c3                   	ret    
 1ea:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 1f0:	31 c0                	xor    %eax,%eax
  return (uchar)*p - (uchar)*q;
 1f2:	29 d8                	sub    %ebx,%eax
}
 1f4:	5b                   	pop    %ebx
 1f5:	5d                   	pop    %ebp
 1f6:	c3                   	ret    
 1f7:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 1fe:	66 90                	xchg   %ax,%ax

00000200 <strlen>:

uint
strlen(const char *s)
{
 200:	f3 0f 1e fb          	endbr32 
 204:	55                   	push   %ebp
 205:	89 e5                	mov    %esp,%ebp
 207:	8b 55 08             	mov    0x8(%ebp),%edx
  int n;

  for(n = 0; s[n]; n++)
 20a:	80 3a 00             	cmpb   $0x0,(%edx)
 20d:	74 21                	je     230 <strlen+0x30>
 20f:	31 c0                	xor    %eax,%eax
 211:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 218:	83 c0 01             	add    $0x1,%eax
 21b:	80 3c 02 00          	cmpb   $0x0,(%edx,%eax,1)
 21f:	89 c1                	mov    %eax,%ecx
 221:	75 f5                	jne    218 <strlen+0x18>
    ;
  return n;
}
 223:	89 c8                	mov    %ecx,%eax
 225:	5d                   	pop    %ebp
 226:	c3                   	ret    
 227:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 22e:	66 90                	xchg   %ax,%ax
  for(n = 0; s[n]; n++)
 230:	31 c9                	xor    %ecx,%ecx
}
 232:	5d                   	pop    %ebp
 233:	89 c8                	mov    %ecx,%eax
 235:	c3                   	ret    
 236:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 23d:	8d 76 00             	lea    0x0(%esi),%esi

00000240 <memset>:

void*
memset(void *dst, int c, uint n)
{
 240:	f3 0f 1e fb          	endbr32 
 244:	55                   	push   %ebp
 245:	89 e5                	mov    %esp,%ebp
 247:	57                   	push   %edi
 248:	8b 55 08             	mov    0x8(%ebp),%edx
}

static inline void
stosb(void *addr, int data, int cnt)
{
  asm volatile("cld; rep stosb" :
 24b:	8b 4d 10             	mov    0x10(%ebp),%ecx
 24e:	8b 45 0c             	mov    0xc(%ebp),%eax
 251:	89 d7                	mov    %edx,%edi
 253:	fc                   	cld    
 254:	f3 aa                	rep stos %al,%es:(%edi)
  stosb(dst, c, n);
  return dst;
}
 256:	89 d0                	mov    %edx,%eax
 258:	5f                   	pop    %edi
 259:	5d                   	pop    %ebp
 25a:	c3                   	ret    
 25b:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 25f:	90                   	nop

00000260 <strchr>:

char*
strchr(const char *s, char c)
{
 260:	f3 0f 1e fb          	endbr32 
 264:	55                   	push   %ebp
 265:	89 e5                	mov    %esp,%ebp
 267:	8b 45 08             	mov    0x8(%ebp),%eax
 26a:	0f b6 4d 0c          	movzbl 0xc(%ebp),%ecx
  for(; *s; s++)
 26e:	0f b6 10             	movzbl (%eax),%edx
 271:	84 d2                	test   %dl,%dl
 273:	75 16                	jne    28b <strchr+0x2b>
 275:	eb 21                	jmp    298 <strchr+0x38>
 277:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 27e:	66 90                	xchg   %ax,%ax
 280:	0f b6 50 01          	movzbl 0x1(%eax),%edx
 284:	83 c0 01             	add    $0x1,%eax
 287:	84 d2                	test   %dl,%dl
 289:	74 0d                	je     298 <strchr+0x38>
    if(*s == c)
 28b:	38 d1                	cmp    %dl,%cl
 28d:	75 f1                	jne    280 <strchr+0x20>
      return (char*)s;
  return 0;
}
 28f:	5d                   	pop    %ebp
 290:	c3                   	ret    
 291:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  return 0;
 298:	31 c0                	xor    %eax,%eax
}
 29a:	5d                   	pop    %ebp
 29b:	c3                   	ret    
 29c:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi

000002a0 <gets>:

char*
gets(char *buf, int max)
{
 2a0:	f3 0f 1e fb          	endbr32 
 2a4:	55                   	push   %ebp
 2a5:	89 e5                	mov    %esp,%ebp
 2a7:	57                   	push   %edi
 2a8:	56                   	push   %esi
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
 2a9:	31 f6                	xor    %esi,%esi
{
 2ab:	53                   	push   %ebx
 2ac:	89 f3                	mov    %esi,%ebx
 2ae:	83 ec 1c             	sub    $0x1c,%esp
 2b1:	8b 7d 08             	mov    0x8(%ebp),%edi
  for(i=0; i+1 < max; ){
 2b4:	eb 33                	jmp    2e9 <gets+0x49>
 2b6:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 2bd:	8d 76 00             	lea    0x0(%esi),%esi
    cc = read(0, &c, 1);
 2c0:	83 ec 04             	sub    $0x4,%esp
 2c3:	8d 45 e7             	lea    -0x19(%ebp),%eax
 2c6:	6a 01                	push   $0x1
 2c8:	50                   	push   %eax
 2c9:	6a 00                	push   $0x0
 2cb:	e8 2b 01 00 00       	call   3fb <read>
    if(cc < 1)
 2d0:	83 c4 10             	add    $0x10,%esp
 2d3:	85 c0                	test   %eax,%eax
 2d5:	7e 1c                	jle    2f3 <gets+0x53>
      break;
    buf[i++] = c;
 2d7:	0f b6 45 e7          	movzbl -0x19(%ebp),%eax
 2db:	83 c7 01             	add    $0x1,%edi
 2de:	88 47 ff             	mov    %al,-0x1(%edi)
    if(c == '\n' || c == '\r')
 2e1:	3c 0a                	cmp    $0xa,%al
 2e3:	74 23                	je     308 <gets+0x68>
 2e5:	3c 0d                	cmp    $0xd,%al
 2e7:	74 1f                	je     308 <gets+0x68>
  for(i=0; i+1 < max; ){
 2e9:	83 c3 01             	add    $0x1,%ebx
 2ec:	89 fe                	mov    %edi,%esi
 2ee:	3b 5d 0c             	cmp    0xc(%ebp),%ebx
 2f1:	7c cd                	jl     2c0 <gets+0x20>
 2f3:	89 f3                	mov    %esi,%ebx
      break;
  }
  buf[i] = '\0';
  return buf;
}
 2f5:	8b 45 08             	mov    0x8(%ebp),%eax
  buf[i] = '\0';
 2f8:	c6 03 00             	movb   $0x0,(%ebx)
}
 2fb:	8d 65 f4             	lea    -0xc(%ebp),%esp
 2fe:	5b                   	pop    %ebx
 2ff:	5e                   	pop    %esi
 300:	5f                   	pop    %edi
 301:	5d                   	pop    %ebp
 302:	c3                   	ret    
 303:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 307:	90                   	nop
 308:	8b 75 08             	mov    0x8(%ebp),%esi
 30b:	8b 45 08             	mov    0x8(%ebp),%eax
 30e:	01 de                	add    %ebx,%esi
 310:	89 f3                	mov    %esi,%ebx
  buf[i] = '\0';
 312:	c6 03 00             	movb   $0x0,(%ebx)
}
 315:	8d 65 f4             	lea    -0xc(%ebp),%esp
 318:	5b                   	pop    %ebx
 319:	5e                   	pop    %esi
 31a:	5f                   	pop    %edi
 31b:	5d                   	pop    %ebp
 31c:	c3                   	ret    
 31d:	8d 76 00             	lea    0x0(%esi),%esi

00000320 <stat>:

int
stat(const char *n, struct stat *st)
{
 320:	f3 0f 1e fb          	endbr32 
 324:	55                   	push   %ebp
 325:	89 e5                	mov    %esp,%ebp
 327:	56                   	push   %esi
 328:	53                   	push   %ebx
  int fd;
  int r;

  fd = open(n, O_RDONLY);
 329:	83 ec 08             	sub    $0x8,%esp
 32c:	6a 00                	push   $0x0
 32e:	ff 75 08             	pushl  0x8(%ebp)
 331:	e8 ed 00 00 00       	call   423 <open>
  if(fd < 0)
 336:	83 c4 10             	add    $0x10,%esp
 339:	85 c0                	test   %eax,%eax
 33b:	78 2b                	js     368 <stat+0x48>
    return -1;
  r = fstat(fd, st);
 33d:	83 ec 08             	sub    $0x8,%esp
 340:	ff 75 0c             	pushl  0xc(%ebp)
 343:	89 c3                	mov    %eax,%ebx
 345:	50                   	push   %eax
 346:	e8 f0 00 00 00       	call   43b <fstat>
  close(fd);
 34b:	89 1c 24             	mov    %ebx,(%esp)
  r = fstat(fd, st);
 34e:	89 c6                	mov    %eax,%esi
  close(fd);
 350:	e8 b6 00 00 00       	call   40b <close>
  return r;
 355:	83 c4 10             	add    $0x10,%esp
}
 358:	8d 65 f8             	lea    -0x8(%ebp),%esp
 35b:	89 f0                	mov    %esi,%eax
 35d:	5b                   	pop    %ebx
 35e:	5e                   	pop    %esi
 35f:	5d                   	pop    %ebp
 360:	c3                   	ret    
 361:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
    return -1;
 368:	be ff ff ff ff       	mov    $0xffffffff,%esi
 36d:	eb e9                	jmp    358 <stat+0x38>
 36f:	90                   	nop

00000370 <atoi>:

int
atoi(const char *s)
{
 370:	f3 0f 1e fb          	endbr32 
 374:	55                   	push   %ebp
 375:	89 e5                	mov    %esp,%ebp
 377:	53                   	push   %ebx
 378:	8b 55 08             	mov    0x8(%ebp),%edx
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
 37b:	0f be 02             	movsbl (%edx),%eax
 37e:	8d 48 d0             	lea    -0x30(%eax),%ecx
 381:	80 f9 09             	cmp    $0x9,%cl
  n = 0;
 384:	b9 00 00 00 00       	mov    $0x0,%ecx
  while('0' <= *s && *s <= '9')
 389:	77 1a                	ja     3a5 <atoi+0x35>
 38b:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 38f:	90                   	nop
    n = n*10 + *s++ - '0';
 390:	83 c2 01             	add    $0x1,%edx
 393:	8d 0c 89             	lea    (%ecx,%ecx,4),%ecx
 396:	8d 4c 48 d0          	lea    -0x30(%eax,%ecx,2),%ecx
  while('0' <= *s && *s <= '9')
 39a:	0f be 02             	movsbl (%edx),%eax
 39d:	8d 58 d0             	lea    -0x30(%eax),%ebx
 3a0:	80 fb 09             	cmp    $0x9,%bl
 3a3:	76 eb                	jbe    390 <atoi+0x20>
  return n;
}
 3a5:	89 c8                	mov    %ecx,%eax
 3a7:	5b                   	pop    %ebx
 3a8:	5d                   	pop    %ebp
 3a9:	c3                   	ret    
 3aa:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi

000003b0 <memmove>:

void*
memmove(void *vdst, const void *vsrc, int n)
{
 3b0:	f3 0f 1e fb          	endbr32 
 3b4:	55                   	push   %ebp
 3b5:	89 e5                	mov    %esp,%ebp
 3b7:	57                   	push   %edi
 3b8:	8b 45 10             	mov    0x10(%ebp),%eax
 3bb:	8b 55 08             	mov    0x8(%ebp),%edx
 3be:	56                   	push   %esi
 3bf:	8b 75 0c             	mov    0xc(%ebp),%esi
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  while(n-- > 0)
 3c2:	85 c0                	test   %eax,%eax
 3c4:	7e 0f                	jle    3d5 <memmove+0x25>
 3c6:	01 d0                	add    %edx,%eax
  dst = vdst;
 3c8:	89 d7                	mov    %edx,%edi
 3ca:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
    *dst++ = *src++;
 3d0:	a4                   	movsb  %ds:(%esi),%es:(%edi)
  while(n-- > 0)
 3d1:	39 f8                	cmp    %edi,%eax
 3d3:	75 fb                	jne    3d0 <memmove+0x20>
  return vdst;
}
 3d5:	5e                   	pop    %esi
 3d6:	89 d0                	mov    %edx,%eax
 3d8:	5f                   	pop    %edi
 3d9:	5d                   	pop    %ebp
 3da:	c3                   	ret    

000003db <fork>:
  name: \
    movl $SYS_ ## name, %eax; \
    int $T_SYSCALL; \
    ret

SYSCALL(fork)
 3db:	b8 01 00 00 00       	mov    $0x1,%eax
 3e0:	cd 40                	int    $0x40
 3e2:	c3                   	ret    

000003e3 <exit>:
SYSCALL(exit)
 3e3:	b8 02 00 00 00       	mov    $0x2,%eax
 3e8:	cd 40                	int    $0x40
 3ea:	c3                   	ret    

000003eb <wait>:
SYSCALL(wait)
 3eb:	b8 03 00 00 00       	mov    $0x3,%eax
 3f0:	cd 40                	int    $0x40
 3f2:	c3                   	ret    

000003f3 <pipe>:
SYSCALL(pipe)
 3f3:	b8 04 00 00 00       	mov    $0x4,%eax
 3f8:	cd 40                	int    $0x40
 3fa:	c3                   	ret    

000003fb <read>:
SYSCALL(read)
 3fb:	b8 05 00 00 00       	mov    $0x5,%eax
 400:	cd 40                	int    $0x40
 402:	c3                   	ret    

00000403 <write>:
SYSCALL(write)
 403:	b8 10 00 00 00       	mov    $0x10,%eax
 408:	cd 40                	int    $0x40
 40a:	c3                   	ret    

0000040b <close>:
SYSCALL(close)
 40b:	b8 15 00 00 00       	mov    $0x15,%eax
 410:	cd 40                	int    $0x40
 412:	c3                   	ret    

00000413 <kill>:
SYSCALL(kill)
 413:	b8 06 00 00 00       	mov    $0x6,%eax
 418:	cd 40                	int    $0x40
 41a:	c3                   	ret    

0000041b <exec>:
SYSCALL(exec)
 41b:	b8 07 00 00 00       	mov    $0x7,%eax
 420:	cd 40                	int    $0x40
 422:	c3                   	ret    

00000423 <open>:
SYSCALL(open)
 423:	b8 0f 00 00 00       	mov    $0xf,%eax
 428:	cd 40                	int    $0x40
 42a:	c3                   	ret    

0000042b <mknod>:
SYSCALL(mknod)
 42b:	b8 11 00 00 00       	mov    $0x11,%eax
 430:	cd 40                	int    $0x40
 432:	c3                   	ret    

00000433 <unlink>:
SYSCALL(unlink)
 433:	b8 12 00 00 00       	mov    $0x12,%eax
 438:	cd 40                	int    $0x40
 43a:	c3                   	ret    

0000043b <fstat>:
SYSCALL(fstat)
 43b:	b8 08 00 00 00       	mov    $0x8,%eax
 440:	cd 40                	int    $0x40
 442:	c3                   	ret    

00000443 <link>:
SYSCALL(link)
 443:	b8 13 00 00 00       	mov    $0x13,%eax
 448:	cd 40                	int    $0x40
 44a:	c3                   	ret    

0000044b <mkdir>:
SYSCALL(mkdir)
 44b:	b8 14 00 00 00       	mov    $0x14,%eax
 450:	cd 40                	int    $0x40
 452:	c3                   	ret    

00000453 <chdir>:
SYSCALL(chdir)
 453:	b8 09 00 00 00       	mov    $0x9,%eax
 458:	cd 40                	int    $0x40
 45a:	c3                   	ret    

0000045b <dup>:
SYSCALL(dup)
 45b:	b8 0a 00 00 00       	mov    $0xa,%eax
 460:	cd 40                	int    $0x40
 462:	c3                   	ret    

00000463 <getpid>:
SYSCALL(getpid)
 463:	b8 0b 00 00 00       	mov    $0xb,%eax
 468:	cd 40                	int    $0x40
 46a:	c3                   	ret    

0000046b <sbrk>:
SYSCALL(sbrk)
 46b:	b8 0c 00 00 00       	mov    $0xc,%eax
 470:	cd 40                	int    $0x40
 472:	c3                   	ret    

00000473 <sleep>:
SYSCALL(sleep)
 473:	b8 0d 00 00 00       	mov    $0xd,%eax
 478:	cd 40                	int    $0x40
 47a:	c3                   	ret    

0000047b <uptime>:
SYSCALL(uptime)
 47b:	b8 0e 00 00 00       	mov    $0xe,%eax
 480:	cd 40                	int    $0x40
 482:	c3                   	ret    
 483:	66 90                	xchg   %ax,%ax
 485:	66 90                	xchg   %ax,%ax
 487:	66 90                	xchg   %ax,%ax
 489:	66 90                	xchg   %ax,%ax
 48b:	66 90                	xchg   %ax,%ax
 48d:	66 90                	xchg   %ax,%ax
 48f:	90                   	nop

00000490 <printint>:
  write(fd, &c, 1);
}

static void
printint(int fd, int xx, int base, int sgn)
{
 490:	55                   	push   %ebp
 491:	89 e5                	mov    %esp,%ebp
 493:	57                   	push   %edi
 494:	56                   	push   %esi
 495:	53                   	push   %ebx
 496:	83 ec 3c             	sub    $0x3c,%esp
 499:	89 4d c4             	mov    %ecx,-0x3c(%ebp)
  uint x;

  neg = 0;
  if(sgn && xx < 0){
    neg = 1;
    x = -xx;
 49c:	89 d1                	mov    %edx,%ecx
{
 49e:	89 45 b8             	mov    %eax,-0x48(%ebp)
  if(sgn && xx < 0){
 4a1:	85 d2                	test   %edx,%edx
 4a3:	0f 89 7f 00 00 00    	jns    528 <printint+0x98>
 4a9:	f6 45 08 01          	testb  $0x1,0x8(%ebp)
 4ad:	74 79                	je     528 <printint+0x98>
    neg = 1;
 4af:	c7 45 bc 01 00 00 00 	movl   $0x1,-0x44(%ebp)
    x = -xx;
 4b6:	f7 d9                	neg    %ecx
  } else {
    x = xx;
  }

  i = 0;
 4b8:	31 db                	xor    %ebx,%ebx
 4ba:	8d 75 d7             	lea    -0x29(%ebp),%esi
 4bd:	8d 76 00             	lea    0x0(%esi),%esi
  do{
    buf[i++] = digits[x % base];
 4c0:	89 c8                	mov    %ecx,%eax
 4c2:	31 d2                	xor    %edx,%edx
 4c4:	89 cf                	mov    %ecx,%edi
 4c6:	f7 75 c4             	divl   -0x3c(%ebp)
 4c9:	0f b6 92 cc 08 00 00 	movzbl 0x8cc(%edx),%edx
 4d0:	89 45 c0             	mov    %eax,-0x40(%ebp)
 4d3:	89 d8                	mov    %ebx,%eax
 4d5:	8d 5b 01             	lea    0x1(%ebx),%ebx
  }while((x /= base) != 0);
 4d8:	8b 4d c0             	mov    -0x40(%ebp),%ecx
    buf[i++] = digits[x % base];
 4db:	88 14 1e             	mov    %dl,(%esi,%ebx,1)
  }while((x /= base) != 0);
 4de:	39 7d c4             	cmp    %edi,-0x3c(%ebp)
 4e1:	76 dd                	jbe    4c0 <printint+0x30>
  if(neg)
 4e3:	8b 4d bc             	mov    -0x44(%ebp),%ecx
 4e6:	85 c9                	test   %ecx,%ecx
 4e8:	74 0c                	je     4f6 <printint+0x66>
    buf[i++] = '-';
 4ea:	c6 44 1d d8 2d       	movb   $0x2d,-0x28(%ebp,%ebx,1)
    buf[i++] = digits[x % base];
 4ef:	89 d8                	mov    %ebx,%eax
    buf[i++] = '-';
 4f1:	ba 2d 00 00 00       	mov    $0x2d,%edx

  while(--i >= 0)
 4f6:	8b 7d b8             	mov    -0x48(%ebp),%edi
 4f9:	8d 5c 05 d7          	lea    -0x29(%ebp,%eax,1),%ebx
 4fd:	eb 07                	jmp    506 <printint+0x76>
 4ff:	90                   	nop
 500:	0f b6 13             	movzbl (%ebx),%edx
 503:	83 eb 01             	sub    $0x1,%ebx
  write(fd, &c, 1);
 506:	83 ec 04             	sub    $0x4,%esp
 509:	88 55 d7             	mov    %dl,-0x29(%ebp)
 50c:	6a 01                	push   $0x1
 50e:	56                   	push   %esi
 50f:	57                   	push   %edi
 510:	e8 ee fe ff ff       	call   403 <write>
  while(--i >= 0)
 515:	83 c4 10             	add    $0x10,%esp
 518:	39 de                	cmp    %ebx,%esi
 51a:	75 e4                	jne    500 <printint+0x70>
    putc(fd, buf[i]);
}
 51c:	8d 65 f4             	lea    -0xc(%ebp),%esp
 51f:	5b                   	pop    %ebx
 520:	5e                   	pop    %esi
 521:	5f                   	pop    %edi
 522:	5d                   	pop    %ebp
 523:	c3                   	ret    
 524:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
  neg = 0;
 528:	c7 45 bc 00 00 00 00 	movl   $0x0,-0x44(%ebp)
 52f:	eb 87                	jmp    4b8 <printint+0x28>
 531:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 538:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 53f:	90                   	nop

00000540 <printf>:

// Print to the given fd. Only understands %d, %x, %p, %s.
void
printf(int fd, const char *fmt, ...)
{
 540:	f3 0f 1e fb          	endbr32 
 544:	55                   	push   %ebp
 545:	89 e5                	mov    %esp,%ebp
 547:	57                   	push   %edi
 548:	56                   	push   %esi
 549:	53                   	push   %ebx
 54a:	83 ec 2c             	sub    $0x2c,%esp
  int c, i, state;
  uint *ap;

  state = 0;
  ap = (uint*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++){
 54d:	8b 75 0c             	mov    0xc(%ebp),%esi
 550:	0f b6 1e             	movzbl (%esi),%ebx
 553:	84 db                	test   %bl,%bl
 555:	0f 84 b4 00 00 00    	je     60f <printf+0xcf>
  ap = (uint*)(void*)&fmt + 1;
 55b:	8d 45 10             	lea    0x10(%ebp),%eax
 55e:	83 c6 01             	add    $0x1,%esi
  write(fd, &c, 1);
 561:	8d 7d e7             	lea    -0x19(%ebp),%edi
  state = 0;
 564:	31 d2                	xor    %edx,%edx
  ap = (uint*)(void*)&fmt + 1;
 566:	89 45 d0             	mov    %eax,-0x30(%ebp)
 569:	eb 33                	jmp    59e <printf+0x5e>
 56b:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 56f:	90                   	nop
 570:	89 55 d4             	mov    %edx,-0x2c(%ebp)
    c = fmt[i] & 0xff;
    if(state == 0){
      if(c == '%'){
        state = '%';
 573:	ba 25 00 00 00       	mov    $0x25,%edx
      if(c == '%'){
 578:	83 f8 25             	cmp    $0x25,%eax
 57b:	74 17                	je     594 <printf+0x54>
  write(fd, &c, 1);
 57d:	83 ec 04             	sub    $0x4,%esp
 580:	88 5d e7             	mov    %bl,-0x19(%ebp)
 583:	6a 01                	push   $0x1
 585:	57                   	push   %edi
 586:	ff 75 08             	pushl  0x8(%ebp)
 589:	e8 75 fe ff ff       	call   403 <write>
 58e:	8b 55 d4             	mov    -0x2c(%ebp),%edx
      } else {
        putc(fd, c);
 591:	83 c4 10             	add    $0x10,%esp
  for(i = 0; fmt[i]; i++){
 594:	0f b6 1e             	movzbl (%esi),%ebx
 597:	83 c6 01             	add    $0x1,%esi
 59a:	84 db                	test   %bl,%bl
 59c:	74 71                	je     60f <printf+0xcf>
    c = fmt[i] & 0xff;
 59e:	0f be cb             	movsbl %bl,%ecx
 5a1:	0f b6 c3             	movzbl %bl,%eax
    if(state == 0){
 5a4:	85 d2                	test   %edx,%edx
 5a6:	74 c8                	je     570 <printf+0x30>
      }
    } else if(state == '%'){
 5a8:	83 fa 25             	cmp    $0x25,%edx
 5ab:	75 e7                	jne    594 <printf+0x54>
      if(c == 'd'){
 5ad:	83 f8 64             	cmp    $0x64,%eax
 5b0:	0f 84 9a 00 00 00    	je     650 <printf+0x110>
        printint(fd, *ap, 10, 1);
        ap++;
      } else if(c == 'x' || c == 'p'){
 5b6:	81 e1 f7 00 00 00    	and    $0xf7,%ecx
 5bc:	83 f9 70             	cmp    $0x70,%ecx
 5bf:	74 5f                	je     620 <printf+0xe0>
        printint(fd, *ap, 16, 0);
        ap++;
      } else if(c == 's'){
 5c1:	83 f8 73             	cmp    $0x73,%eax
 5c4:	0f 84 d6 00 00 00    	je     6a0 <printf+0x160>
          s = "(null)";
        while(*s != 0){
          putc(fd, *s);
          s++;
        }
      } else if(c == 'c'){
 5ca:	83 f8 63             	cmp    $0x63,%eax
 5cd:	0f 84 8d 00 00 00    	je     660 <printf+0x120>
        putc(fd, *ap);
        ap++;
      } else if(c == '%'){
 5d3:	83 f8 25             	cmp    $0x25,%eax
 5d6:	0f 84 b4 00 00 00    	je     690 <printf+0x150>
  write(fd, &c, 1);
 5dc:	83 ec 04             	sub    $0x4,%esp
 5df:	c6 45 e7 25          	movb   $0x25,-0x19(%ebp)
 5e3:	6a 01                	push   $0x1
 5e5:	57                   	push   %edi
 5e6:	ff 75 08             	pushl  0x8(%ebp)
 5e9:	e8 15 fe ff ff       	call   403 <write>
        putc(fd, c);
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
        putc(fd, c);
 5ee:	88 5d e7             	mov    %bl,-0x19(%ebp)
  write(fd, &c, 1);
 5f1:	83 c4 0c             	add    $0xc,%esp
 5f4:	6a 01                	push   $0x1
 5f6:	83 c6 01             	add    $0x1,%esi
 5f9:	57                   	push   %edi
 5fa:	ff 75 08             	pushl  0x8(%ebp)
 5fd:	e8 01 fe ff ff       	call   403 <write>
  for(i = 0; fmt[i]; i++){
 602:	0f b6 5e ff          	movzbl -0x1(%esi),%ebx
        putc(fd, c);
 606:	83 c4 10             	add    $0x10,%esp
      }
      state = 0;
 609:	31 d2                	xor    %edx,%edx
  for(i = 0; fmt[i]; i++){
 60b:	84 db                	test   %bl,%bl
 60d:	75 8f                	jne    59e <printf+0x5e>
    }
  }
}
 60f:	8d 65 f4             	lea    -0xc(%ebp),%esp
 612:	5b                   	pop    %ebx
 613:	5e                   	pop    %esi
 614:	5f                   	pop    %edi
 615:	5d                   	pop    %ebp
 616:	c3                   	ret    
 617:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 61e:	66 90                	xchg   %ax,%ax
        printint(fd, *ap, 16, 0);
 620:	83 ec 0c             	sub    $0xc,%esp
 623:	b9 10 00 00 00       	mov    $0x10,%ecx
 628:	6a 00                	push   $0x0
 62a:	8b 5d d0             	mov    -0x30(%ebp),%ebx
 62d:	8b 45 08             	mov    0x8(%ebp),%eax
 630:	8b 13                	mov    (%ebx),%edx
 632:	e8 59 fe ff ff       	call   490 <printint>
        ap++;
 637:	89 d8                	mov    %ebx,%eax
 639:	83 c4 10             	add    $0x10,%esp
      state = 0;
 63c:	31 d2                	xor    %edx,%edx
        ap++;
 63e:	83 c0 04             	add    $0x4,%eax
 641:	89 45 d0             	mov    %eax,-0x30(%ebp)
 644:	e9 4b ff ff ff       	jmp    594 <printf+0x54>
 649:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
        printint(fd, *ap, 10, 1);
 650:	83 ec 0c             	sub    $0xc,%esp
 653:	b9 0a 00 00 00       	mov    $0xa,%ecx
 658:	6a 01                	push   $0x1
 65a:	eb ce                	jmp    62a <printf+0xea>
 65c:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
        putc(fd, *ap);
 660:	8b 5d d0             	mov    -0x30(%ebp),%ebx
  write(fd, &c, 1);
 663:	83 ec 04             	sub    $0x4,%esp
        putc(fd, *ap);
 666:	8b 03                	mov    (%ebx),%eax
  write(fd, &c, 1);
 668:	6a 01                	push   $0x1
        ap++;
 66a:	83 c3 04             	add    $0x4,%ebx
  write(fd, &c, 1);
 66d:	57                   	push   %edi
 66e:	ff 75 08             	pushl  0x8(%ebp)
        putc(fd, *ap);
 671:	88 45 e7             	mov    %al,-0x19(%ebp)
  write(fd, &c, 1);
 674:	e8 8a fd ff ff       	call   403 <write>
        ap++;
 679:	89 5d d0             	mov    %ebx,-0x30(%ebp)
 67c:	83 c4 10             	add    $0x10,%esp
      state = 0;
 67f:	31 d2                	xor    %edx,%edx
 681:	e9 0e ff ff ff       	jmp    594 <printf+0x54>
 686:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 68d:	8d 76 00             	lea    0x0(%esi),%esi
        putc(fd, c);
 690:	88 5d e7             	mov    %bl,-0x19(%ebp)
  write(fd, &c, 1);
 693:	83 ec 04             	sub    $0x4,%esp
 696:	e9 59 ff ff ff       	jmp    5f4 <printf+0xb4>
 69b:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 69f:	90                   	nop
        s = (char*)*ap;
 6a0:	8b 45 d0             	mov    -0x30(%ebp),%eax
 6a3:	8b 18                	mov    (%eax),%ebx
        ap++;
 6a5:	83 c0 04             	add    $0x4,%eax
 6a8:	89 45 d0             	mov    %eax,-0x30(%ebp)
        if(s == 0)
 6ab:	85 db                	test   %ebx,%ebx
 6ad:	74 17                	je     6c6 <printf+0x186>
        while(*s != 0){
 6af:	0f b6 03             	movzbl (%ebx),%eax
      state = 0;
 6b2:	31 d2                	xor    %edx,%edx
        while(*s != 0){
 6b4:	84 c0                	test   %al,%al
 6b6:	0f 84 d8 fe ff ff    	je     594 <printf+0x54>
 6bc:	89 75 d4             	mov    %esi,-0x2c(%ebp)
 6bf:	89 de                	mov    %ebx,%esi
 6c1:	8b 5d 08             	mov    0x8(%ebp),%ebx
 6c4:	eb 1a                	jmp    6e0 <printf+0x1a0>
          s = "(null)";
 6c6:	bb c3 08 00 00       	mov    $0x8c3,%ebx
        while(*s != 0){
 6cb:	89 75 d4             	mov    %esi,-0x2c(%ebp)
 6ce:	b8 28 00 00 00       	mov    $0x28,%eax
 6d3:	89 de                	mov    %ebx,%esi
 6d5:	8b 5d 08             	mov    0x8(%ebp),%ebx
 6d8:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 6df:	90                   	nop
  write(fd, &c, 1);
 6e0:	83 ec 04             	sub    $0x4,%esp
          s++;
 6e3:	83 c6 01             	add    $0x1,%esi
 6e6:	88 45 e7             	mov    %al,-0x19(%ebp)
  write(fd, &c, 1);
 6e9:	6a 01                	push   $0x1
 6eb:	57                   	push   %edi
 6ec:	53                   	push   %ebx
 6ed:	e8 11 fd ff ff       	call   403 <write>
        while(*s != 0){
 6f2:	0f b6 06             	movzbl (%esi),%eax
 6f5:	83 c4 10             	add    $0x10,%esp
 6f8:	84 c0                	test   %al,%al
 6fa:	75 e4                	jne    6e0 <printf+0x1a0>
 6fc:	8b 75 d4             	mov    -0x2c(%ebp),%esi
      state = 0;
 6ff:	31 d2                	xor    %edx,%edx
 701:	e9 8e fe ff ff       	jmp    594 <printf+0x54>
 706:	66 90                	xchg   %ax,%ax
 708:	66 90                	xchg   %ax,%ax
 70a:	66 90                	xchg   %ax,%ax
 70c:	66 90                	xchg   %ax,%ax
 70e:	66 90                	xchg   %ax,%ax

00000710 <free>:
static Header base;
static Header *freep;

void
free(void *ap)
{
 710:	f3 0f 1e fb          	endbr32 
 714:	55                   	push   %ebp
  Header *bp, *p;

  bp = (Header*)ap - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 715:	a1 7c 0b 00 00       	mov    0xb7c,%eax
{
 71a:	89 e5                	mov    %esp,%ebp
 71c:	57                   	push   %edi
 71d:	56                   	push   %esi
 71e:	53                   	push   %ebx
 71f:	8b 5d 08             	mov    0x8(%ebp),%ebx
 722:	8b 10                	mov    (%eax),%edx
  bp = (Header*)ap - 1;
 724:	8d 4b f8             	lea    -0x8(%ebx),%ecx
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 727:	39 c8                	cmp    %ecx,%eax
 729:	73 15                	jae    740 <free+0x30>
 72b:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 72f:	90                   	nop
 730:	39 d1                	cmp    %edx,%ecx
 732:	72 14                	jb     748 <free+0x38>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 734:	39 d0                	cmp    %edx,%eax
 736:	73 10                	jae    748 <free+0x38>
{
 738:	89 d0                	mov    %edx,%eax
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 73a:	8b 10                	mov    (%eax),%edx
 73c:	39 c8                	cmp    %ecx,%eax
 73e:	72 f0                	jb     730 <free+0x20>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 740:	39 d0                	cmp    %edx,%eax
 742:	72 f4                	jb     738 <free+0x28>
 744:	39 d1                	cmp    %edx,%ecx
 746:	73 f0                	jae    738 <free+0x28>
      break;
  if(bp + bp->s.size == p->s.ptr){
 748:	8b 73 fc             	mov    -0x4(%ebx),%esi
 74b:	8d 3c f1             	lea    (%ecx,%esi,8),%edi
 74e:	39 fa                	cmp    %edi,%edx
 750:	74 1e                	je     770 <free+0x60>
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr;
 752:	89 53 f8             	mov    %edx,-0x8(%ebx)
  if(p + p->s.size == bp){
 755:	8b 50 04             	mov    0x4(%eax),%edx
 758:	8d 34 d0             	lea    (%eax,%edx,8),%esi
 75b:	39 f1                	cmp    %esi,%ecx
 75d:	74 28                	je     787 <free+0x77>
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;
 75f:	89 08                	mov    %ecx,(%eax)
  freep = p;
}
 761:	5b                   	pop    %ebx
  freep = p;
 762:	a3 7c 0b 00 00       	mov    %eax,0xb7c
}
 767:	5e                   	pop    %esi
 768:	5f                   	pop    %edi
 769:	5d                   	pop    %ebp
 76a:	c3                   	ret    
 76b:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 76f:	90                   	nop
    bp->s.size += p->s.ptr->s.size;
 770:	03 72 04             	add    0x4(%edx),%esi
 773:	89 73 fc             	mov    %esi,-0x4(%ebx)
    bp->s.ptr = p->s.ptr->s.ptr;
 776:	8b 10                	mov    (%eax),%edx
 778:	8b 12                	mov    (%edx),%edx
 77a:	89 53 f8             	mov    %edx,-0x8(%ebx)
  if(p + p->s.size == bp){
 77d:	8b 50 04             	mov    0x4(%eax),%edx
 780:	8d 34 d0             	lea    (%eax,%edx,8),%esi
 783:	39 f1                	cmp    %esi,%ecx
 785:	75 d8                	jne    75f <free+0x4f>
    p->s.size += bp->s.size;
 787:	03 53 fc             	add    -0x4(%ebx),%edx
  freep = p;
 78a:	a3 7c 0b 00 00       	mov    %eax,0xb7c
    p->s.size += bp->s.size;
 78f:	89 50 04             	mov    %edx,0x4(%eax)
    p->s.ptr = bp->s.ptr;
 792:	8b 53 f8             	mov    -0x8(%ebx),%edx
 795:	89 10                	mov    %edx,(%eax)
}
 797:	5b                   	pop    %ebx
 798:	5e                   	pop    %esi
 799:	5f                   	pop    %edi
 79a:	5d                   	pop    %ebp
 79b:	c3                   	ret    
 79c:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi

000007a0 <malloc>:
  return freep;
}

void*
malloc(uint nbytes)
{
 7a0:	f3 0f 1e fb          	endbr32 
 7a4:	55                   	push   %ebp
 7a5:	89 e5                	mov    %esp,%ebp
 7a7:	57                   	push   %edi
 7a8:	56                   	push   %esi
 7a9:	53                   	push   %ebx
 7aa:	83 ec 1c             	sub    $0x1c,%esp
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 7ad:	8b 45 08             	mov    0x8(%ebp),%eax
  if((prevp = freep) == 0){
 7b0:	8b 3d 7c 0b 00 00    	mov    0xb7c,%edi
  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 7b6:	8d 70 07             	lea    0x7(%eax),%esi
 7b9:	c1 ee 03             	shr    $0x3,%esi
 7bc:	83 c6 01             	add    $0x1,%esi
  if((prevp = freep) == 0){
 7bf:	85 ff                	test   %edi,%edi
 7c1:	0f 84 a9 00 00 00    	je     870 <malloc+0xd0>
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 7c7:	8b 07                	mov    (%edi),%eax
    if(p->s.size >= nunits){
 7c9:	8b 48 04             	mov    0x4(%eax),%ecx
 7cc:	39 f1                	cmp    %esi,%ecx
 7ce:	73 6d                	jae    83d <malloc+0x9d>
 7d0:	81 fe 00 10 00 00    	cmp    $0x1000,%esi
 7d6:	bb 00 10 00 00       	mov    $0x1000,%ebx
 7db:	0f 43 de             	cmovae %esi,%ebx
  p = sbrk(nu * sizeof(Header));
 7de:	8d 0c dd 00 00 00 00 	lea    0x0(,%ebx,8),%ecx
 7e5:	89 4d e4             	mov    %ecx,-0x1c(%ebp)
 7e8:	eb 17                	jmp    801 <malloc+0x61>
 7ea:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 7f0:	8b 10                	mov    (%eax),%edx
    if(p->s.size >= nunits){
 7f2:	8b 4a 04             	mov    0x4(%edx),%ecx
 7f5:	39 f1                	cmp    %esi,%ecx
 7f7:	73 4f                	jae    848 <malloc+0xa8>
 7f9:	8b 3d 7c 0b 00 00    	mov    0xb7c,%edi
 7ff:	89 d0                	mov    %edx,%eax
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
 801:	39 c7                	cmp    %eax,%edi
 803:	75 eb                	jne    7f0 <malloc+0x50>
  p = sbrk(nu * sizeof(Header));
 805:	83 ec 0c             	sub    $0xc,%esp
 808:	ff 75 e4             	pushl  -0x1c(%ebp)
 80b:	e8 5b fc ff ff       	call   46b <sbrk>
  if(p == (char*)-1)
 810:	83 c4 10             	add    $0x10,%esp
 813:	83 f8 ff             	cmp    $0xffffffff,%eax
 816:	74 1b                	je     833 <malloc+0x93>
  hp->s.size = nu;
 818:	89 58 04             	mov    %ebx,0x4(%eax)
  free((void*)(hp + 1));
 81b:	83 ec 0c             	sub    $0xc,%esp
 81e:	83 c0 08             	add    $0x8,%eax
 821:	50                   	push   %eax
 822:	e8 e9 fe ff ff       	call   710 <free>
  return freep;
 827:	a1 7c 0b 00 00       	mov    0xb7c,%eax
      if((p = morecore(nunits)) == 0)
 82c:	83 c4 10             	add    $0x10,%esp
 82f:	85 c0                	test   %eax,%eax
 831:	75 bd                	jne    7f0 <malloc+0x50>
        return 0;
  }
}
 833:	8d 65 f4             	lea    -0xc(%ebp),%esp
        return 0;
 836:	31 c0                	xor    %eax,%eax
}
 838:	5b                   	pop    %ebx
 839:	5e                   	pop    %esi
 83a:	5f                   	pop    %edi
 83b:	5d                   	pop    %ebp
 83c:	c3                   	ret    
    if(p->s.size >= nunits){
 83d:	89 c2                	mov    %eax,%edx
 83f:	89 f8                	mov    %edi,%eax
 841:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
      if(p->s.size == nunits)
 848:	39 ce                	cmp    %ecx,%esi
 84a:	74 54                	je     8a0 <malloc+0x100>
        p->s.size -= nunits;
 84c:	29 f1                	sub    %esi,%ecx
 84e:	89 4a 04             	mov    %ecx,0x4(%edx)
        p += p->s.size;
 851:	8d 14 ca             	lea    (%edx,%ecx,8),%edx
        p->s.size = nunits;
 854:	89 72 04             	mov    %esi,0x4(%edx)
      freep = prevp;
 857:	a3 7c 0b 00 00       	mov    %eax,0xb7c
}
 85c:	8d 65 f4             	lea    -0xc(%ebp),%esp
      return (void*)(p + 1);
 85f:	8d 42 08             	lea    0x8(%edx),%eax
}
 862:	5b                   	pop    %ebx
 863:	5e                   	pop    %esi
 864:	5f                   	pop    %edi
 865:	5d                   	pop    %ebp
 866:	c3                   	ret    
 867:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 86e:	66 90                	xchg   %ax,%ax
    base.s.ptr = freep = prevp = &base;
 870:	c7 05 7c 0b 00 00 80 	movl   $0xb80,0xb7c
 877:	0b 00 00 
    base.s.size = 0;
 87a:	bf 80 0b 00 00       	mov    $0xb80,%edi
    base.s.ptr = freep = prevp = &base;
 87f:	c7 05 80 0b 00 00 80 	movl   $0xb80,0xb80
 886:	0b 00 00 
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 889:	89 f8                	mov    %edi,%eax
    base.s.size = 0;
 88b:	c7 05 84 0b 00 00 00 	movl   $0x0,0xb84
 892:	00 00 00 
    if(p->s.size >= nunits){
 895:	e9 36 ff ff ff       	jmp    7d0 <malloc+0x30>
 89a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
        prevp->s.ptr = p->s.ptr;
 8a0:	8b 0a                	mov    (%edx),%ecx
 8a2:	89 08                	mov    %ecx,(%eax)
 8a4:	eb b1                	jmp    857 <malloc+0xb7>
