void kprint(const char* msg)
{
    static char* vram = (char*)0xB8000;

    for (int i =0; msg && msg[i]; i += 2)
    {
        vram[i] = msg[i];
    }
}

void kmain()
{
    kprint("Hello from kernel");
}