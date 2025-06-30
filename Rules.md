# Important Rules

---

## 1. Arithmetic Progression (AP)

An **arithmetic progression** is a sequence of numbers in which the difference between any two successive members is constant.

### 1.1. Definitions

- **First term**:  
  $$a_1$$
- **Common difference**:  
  $$d = a_n - a_{n-1}$$

### 1.2. \(n\)th Term

- **Formula**:  
  $$a_n = a_1 + (n - 1)\,d$$

### 1.3. Sum of First \(n\) Terms

- **Sum**:  
  $$
  S_n = \frac{n}{2}\,(a_1 + a_n)
  = \frac{n}{2}\,\bigl[2\,a_1 + (n-1)\,d\bigr]
  $$

---

## 2. Geometric Progression (GP)

A **geometric progression** is a sequence where each term after the first is found by multiplying the previous one by a fixed, nonzero number called the **common ratio**.

### 2.1. Definitions

- **First term**:  
  $$g_1$$
- **Common ratio**:  
  $$r = \frac{g_n}{g_{n-1}}$$

### 2.2. \(n\)th Term

- **Formula**:  
  $$g_n = g_1\,r^{\,n-1}$$

### 2.3. Sum of First \(n\) Terms

- **If \(r != 1\):**  
  $$
  S_n = g_1\,\frac{r^n - 1}{r - 1}
  $$
- **If \(|r| < 1\):**  
  $$
  S_\infty = \frac{g_1}{1 - r}
  $$

---

## 3. Divisibility Rules

Quick checks to determine if an integer is divisible by small numbers:

| Divisor | Rule                                                                                          |
|:-------:|:----------------------------------------------------------------------------------------------|
| **2**   | Last digit is even (0, 2, 4, 6, 8).                                                           |
| **3**   | Sum of digits is divisible by 3.                                                              |
| **4**   | Last two digits form a number divisible by 4.                                                 |
| **5**   | Last digit is 0 or 5.                                                                         |
| **6**   | Divisible by **2** and **3**.                                                                 |
| **7**   | Double the last digit, subtract it from the truncated leading number; if result divisible by 7. |
| **8**   | Last three digits form a number divisible by 8.                                               |
| **9**   | Sum of digits is divisible by 9.                                                              |
| **10**  | Last digit is 0.                                                                              |
| **11**  | Alternating sum of digits (odd positions minus even) is divisible by 11.                      |
| **12**  | Divisible by **3** and **4**.                                                                 |

### 3.1. Examples

```text
234 divisible by 2?  → last digit 4 → yes
234 divisible by 3?  → 2 + 3 + 4 = 9; 9 mod 3 = 0 → yes
234 divisible by 4?  → last two digits 34; 34 mod 4 = 2 → no
234 divisible by 6?  → divisible by 2 & 3 → yes
```
