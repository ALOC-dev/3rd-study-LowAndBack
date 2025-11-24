# 3rd-study-LowAndBack

## 🧠 프로젝트 소개
'로우레벨 프로그래밍 근데 이제 백엔드를 곁들인'팀의 시스템 프로그래밍 스터디입니다.
C언어로 Mini Shell과 Mini DBMS를 구현하면서 운영체제와 백엔드 시스템을 이해합니다.



## 🗓️ 개발 기간
2025-04-02 ~ 2025-06-22



## 🎯 최종 목표
- **나만의 Unix Shell**을 직접 구현
- `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit` 같은 **내장 명령어** 직접 구현
- `|`, `<`, `>`, `>>`, `^C`, `^D` 등의 **파이프, 리디렉션, 시그널** 제어 처리
- **Mini DBMS 구현 및 Shell과 연동**
- 포트폴리오로 제출 가능한 실전 수준 코드베이스 구성



## 👥 팀원 소개
<table>
  <tr>
    <td align="center">
      <a href="https://github.com/jigun058">
        <img src="https://github.com/jigun058.png" width="100px;" alt="jigun058"/><br />
        <sub><b>이태권</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/ktollonr">
        <img src="https://github.com/ktollonr.png" width="100px;" alt="ktollonr"/><br />
        <sub><b>최정혁</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/uukdo">
        <img src="https://github.com/uukdo.png" width="100px;" alt="uukdo"/><br />
        <sub><b>장지원</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/sungj1n">
        <img src="https://github.com/sungj1n.png" width="100px;" alt="sungj1n"/><br />
        <sub><b>박성진</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/in-su03">
        <img src="https://github.com/in-su03.png" width="100px;" alt="in-su03"/><br />
        <sub><b>배인수</b></sub>
      </a>
    </td>
  </tr>
</table>



## 🛠 기술 스택
- **언어**: C (GCC)
- **플랫폼**: MacOS
- **툴**: Make, Git
- **기술 키워드**: Shell 구현, 시스템콜, 환경변수, 파싱, DBMS



## 📁 디렉터리 구조

```
minishell/
├── include/              # 전역 헤더 파일
│   └── minishell.h
├── lib/                  # 외부 라이브러리, 공용 유틸 함수
├── src/
│   ├── shell/            # Shell 관련 로직
│   │   ├── parser/       # 명령어 파싱
│   │   ├── builtin/      # 내장 명령어
│   │   ├── executor/     # 명령 실행, 리디렉션, 파이프
│   │   ├── utils/        # 문자열 유틸, 에러 처리
│   │   └── main.c        # Shell 진입점 (REPL)
│   └── dbms/             # DBMS 관련 로직
│       ├── parser/       # SQL-like 쿼리 해석
│       ├── engine/       # 명령 실행 로직
│       ├── storage/      # 파일 기반 저장소
│       └── utils/        # 테이블 유틸 함수
├── Makefile              # 빌드 설정
```



## ⏱️ 스터디 일정

| 주차 | 날짜 | 주제 | 세부 내용 |
|------|------|------|-----------|
| 2주차 | 3/17 | C 기초 학습 1 | 환경 설정 및 간단한 문법 |
| 3주차 | 3/24 | C 기초 학습 2 | 구조체, 포인터 등 복습 |
| 4주차 | 3/31 | 시스템콜 기초 | 프로젝트 셋업, `read`, `write`, `open`, `close` 등 |
| 5주차 | 4/7 | 기본 쉘 구조 | 프롬프트, 명령어 파싱, 실행 |
| 6~7주차 | 미정 | 프로세스 처리 | `fork()`, `exec()`, `wait()` |
| 8주차 | 4/28 | 리디렉션 & 파이프 | `<`, `>`, `|` |
| 9주차 | 5/7 | 백그라운드 실행 | `&` 처리, job control |
| 10주차 | 5/12 | 환경 변수 & 시그널 | `export`, `unset`, Ctrl+C 등 처리 |
| 11주차 | 5/19 | 미니 DBMS 설계 | 테이블 구조, 명령어 파싱 (CREATE, INSERT, SELECT) |
| 12주차 | 5/26 | Shell ↔ DB 연동 | SQL-like 해석 → DB 처리 함수 호출 |
| 13주차 | 6/2 | 고도화 | Persistence, 에러 핸들링, 테스트 |



## 🚀 실행 방법

```bash
# 1. 저장소 클론
git clone https://github.com/ALOC-dev/3rd-study-LowAndBack.git

# 2. 빌드
cd minishell
make

# 3. 실행
./minishell
