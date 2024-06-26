# XorString - Simple string encryption with C++20
C++20 introduced `consteval` which greatly simplifies making this kind of string encryption.

## Example
```c++
int main() {
	printf(XORSTR("Hello, world!\n"));
}
```