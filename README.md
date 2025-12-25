**🗂️ Task Management System in C**

A console-based Task Management System written in C that allows users to manage tasks with deadlines and priorities while receiving automatic reminder notifications. The program continuously checks deadlines in the background and alerts users at critical time intervals.

This project demonstrates classic C programming concepts such as structures, time handling, string parsing, and system-level command execution.

📌 Features

Add tasks with:

Task name

Deadline (YYYY-MM-DD HH:MM)

Priority (High / Medium / Low)

View all added tasks

Delete tasks by index

Background deadline monitoring

Priority-based reminders:

High: 60, 30, and 10 minutes before deadline

Medium: 30 and 10 minutes before deadline

Low: 10 minutes before deadline

Windows popup notifications using PowerShell

🛠️ Technologies Used

Language: C

Libraries:

stdio.h – Input/Output

stdlib.h – Memory and system commands

string.h – String handling

time.h – Deadline tracking

unistd.h – Background delay (sleep)

Platform: Windows (for popup notifications)

⚙️ How It Works

Tasks are stored in a fixed-size array of structures.

Deadlines are parsed into struct tm format.

The program continuously compares current system time with task deadlines.

Reminder flags ensure notifications are sent only once per interval.

Notifications are displayed using Windows MessageBox via PowerShell.

Old-school loop, honest logic, no unnecessary abstractions—just how C likes it.
