extern void kernel_main();  // Real kernel function.

extern "C" void kernel_stage_2()
{

    kernel_main();
    while (true);
}