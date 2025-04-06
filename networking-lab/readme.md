
# 🕒 Understanding `struct timeval` and `gettimeofday()` in C

When building time-sensitive applications like **Selective Repeat ARQ**, we often need precise timing to handle retransmissions and timeouts. C provides us with tools like:

- `struct timeval` – to represent time values
- `gettimeofday()` – to fetch the current time

This guide will explain what each does, how to use them, and give you example code to demonstrate their purpose.

---

## 📦 `struct timeval`

`struct timeval` is a structure defined in `<sys/time.h>`. It holds a time value with **microsecond precision**.

### 🔍 Structure Definition:

```c
struct timeval {
    time_t      tv_sec;   // seconds
    suseconds_t tv_usec;  // microseconds (0 to 999,999)
};
```

### 🧠 What each field does:

| Field      | Description                                        | Example value     |
|------------|----------------------------------------------------|-------------------|
| `tv_sec`   | Whole seconds since the Unix epoch (Jan 1, 1970)   | `1712405647`      |
| `tv_usec`  | Microseconds part of the time (1 sec = 1,000,000 μs) | `589123`          |

Together, they represent a **timestamp accurate to the microsecond**.

---

## 🧪 Example: Using `struct timeval`

```c
#include <stdio.h>
#include <sys/time.h>

int main() {
    struct timeval now;
    gettimeofday(&now, NULL);
    
    printf("Seconds: %ld\n", now.tv_sec);
    printf("Microseconds: %ld\n", now.tv_usec);
    
    return 0;
}
```

### 🖨 Sample Output:
```
Seconds: 1712405647
Microseconds: 589123
```

This means the current time is **1712405647.589123 seconds** since Jan 1, 1970.

---

## 🧩 `gettimeofday()` Function

### 📌 Function Prototype:

```c
int gettimeofday(struct timeval *tv, struct timezone *tz);
```

### 📄 Parameters:
- `tv`: A pointer to `struct timeval` where current time will be stored.
- `tz`: Usually set to `NULL` (timezone handling is obsolete).

### 📄 Return value:
- Returns `0` on success, `-1` on failure.

---

### 🧪 Example: Measuring time elapsed

```c
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
    struct timeval start, end;

    gettimeofday(&start, NULL);
    sleep(2); // Delay for 2 seconds
    gettimeofday(&end, NULL);

    long elapsed = end.tv_sec - start.tv_sec;
    printf("Elapsed time: %ld seconds\n", elapsed);
    
    return 0;
}
```

### 🖨 Output:
```
Elapsed time: 2 seconds
```

This helps simulate **timeouts or delays** between events.

---

## ✅ Practical Use in Selective Repeat ARQ

In protocols like **Selective Repeat**, each frame has a timer to track retransmissions.

### Frame structure:
```c
struct frame {
    int seq;
    int acked;
    struct timeval sent_time; // Timestamp when frame was sent
};
```

### To store send time:
```c
gettimeofday(&frames[i].sent_time, NULL);
```

### To check if timeout occurred:
```c
int time_elapsed(struct timeval start) {
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec - start.tv_sec); // returns seconds passed
}
```

If `time_elapsed()` returns ≥ 3 (our timeout limit), we retransmit the frame.

---

## ⚠️ Note:
- `gettimeofday()` gives wall-clock time, and can be affected by system clock changes.
- For monotonic time (not affected by system updates), prefer `clock_gettime()` with `CLOCK_MONOTONIC`.

---

## 📚 Summary

| Feature             | Description                                 |
|---------------------|---------------------------------------------|
| `struct timeval`    | Time structure with seconds + microseconds  |
| `tv_sec`            | Seconds since epoch                         |
| `tv_usec`           | Microseconds part of the time               |
| `gettimeofday()`    | Fills a timeval struct with current time    |
| Use case            | Measuring elapsed time, timeout handling    |

---
