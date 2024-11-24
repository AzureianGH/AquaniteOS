#include <idt/idt.h>
#include <idt/isr.h>
#include <flanterm/flantermglobal.h>
#include <io/io.h>
#include <pit/pit.h>
#include <string/string.h>
void IRQ0HNDLER(registers_t *r) {
    PITHandler(r);
    scheduler(r);
    //run_next_process();
}

void InitializeISR() {
  SetIDTGate(0, (uint64_t)isr0);
  SetIDTGate(1, (uint64_t)isr1);
  SetIDTGate(2, (uint64_t)isr2);
  SetIDTGate(3, (uint64_t)isr3);
  SetIDTGate(4, (uint64_t)isr4);
  SetIDTGate(5, (uint64_t)isr5);
  SetIDTGate(6, (uint64_t)isr6);
  SetIDTGate(7, (uint64_t)isr7);
  SetIDTGate(8, (uint64_t)errorIsr8);
  SetIDTGate(9, (uint64_t)isr9);
  SetIDTGate(10, (uint64_t)errorIsr10);
  SetIDTGate(11, (uint64_t)errorIsr11);
  SetIDTGate(12, (uint64_t)errorIsr12);
  SetIDTGate(13, (uint64_t)errorIsr13);
  SetIDTGate(14, (uint64_t)errorIsr14);
  SetIDTGate(15, (uint64_t)isr15);
  SetIDTGate(16, (uint64_t)isr16);
  SetIDTGate(17, (uint64_t)isr17);
  SetIDTGate(18, (uint64_t)isr18);
  SetIDTGate(19, (uint64_t)isr19);
  SetIDTGate(20, (uint64_t)isr20);
  SetIDTGate(21, (uint64_t)isr21);
  SetIDTGate(22, (uint64_t)isr22);
  SetIDTGate(23, (uint64_t)isr23);
  SetIDTGate(24, (uint64_t)isr24);
  SetIDTGate(25, (uint64_t)isr25);
  SetIDTGate(26, (uint64_t)isr26);
  SetIDTGate(27, (uint64_t)isr27);
  SetIDTGate(28, (uint64_t)isr28);
  SetIDTGate(29, (uint64_t)isr29);
  SetIDTGate(30, (uint64_t)isr30);
  SetIDTGate(31, (uint64_t)isr31);
  outb(0x20, 0x11); 
  outb(0xA0, 0x11);
  outb(0x21, 0x20);
  outb(0xA1, 0x28);
  outb(0x21, 0x04);
  outb(0xA1, 0x02);
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  outb(0x21, 0x0);
  outb(0xA1, 0x0);
  SetIDTGate(32, (uint64_t)isr32);
  SetIDTGate(33, (uint64_t)isr33);
  SetIDTGate(34, (uint64_t)isr34);
  SetIDTGate(35, (uint64_t)isr35);
  SetIDTGate(36, (uint64_t)isr36);
  SetIDTGate(37, (uint64_t)isr37);
  SetIDTGate(38, (uint64_t)isr38);
  SetIDTGate(39, (uint64_t)isr39);
  SetIDTGate(40, (uint64_t)isr40);
  SetIDTGate(41, (uint64_t)isr41);
  SetIDTGate(42, (uint64_t)isr42);
  SetIDTGate(43, (uint64_t)isr43);
  SetIDTGate(44, (uint64_t)isr44);
  SetIDTGate(45, (uint64_t)isr45);
  SetIDTGate(46, (uint64_t)isr46);
  SetIDTGate(47, (uint64_t)isr47);
  SetIDTGate(48, (uint64_t)isr48);
  SetIDTGate(49, (uint64_t)isr49);
  SetIDTGate(50, (uint64_t)isr50);
  SetIDTGate(51, (uint64_t)isr51);
  SetIDTGate(52, (uint64_t)isr52);
  SetIDTGate(53, (uint64_t)isr53);
  SetIDTGate(54, (uint64_t)isr54);
  SetIDTGate(55, (uint64_t)isr55);
  SetIDTGate(56, (uint64_t)isr56);
  SetIDTGate(57, (uint64_t)isr57);
  SetIDTGate(58, (uint64_t)isr58);
  SetIDTGate(59, (uint64_t)isr59);
  SetIDTGate(60, (uint64_t)isr60);
  SetIDTGate(61, (uint64_t)isr61);
  SetIDTGate(62, (uint64_t)isr62);
  SetIDTGate(63, (uint64_t)isr63);
  SetIDTGate(64, (uint64_t)isr64);
  SetIDTGate(65, (uint64_t)isr65);
  SetIDTGate(66, (uint64_t)isr66);
  SetIDTGate(67, (uint64_t)isr67);
  SetIDTGate(68, (uint64_t)isr68);
  SetIDTGate(69, (uint64_t)isr69);
  SetIDTGate(70, (uint64_t)isr70);
  SetIDTGate(71, (uint64_t)isr71);
  SetIDTGate(72, (uint64_t)isr72);
  SetIDTGate(73, (uint64_t)isr73);
  SetIDTGate(74, (uint64_t)isr74);
  SetIDTGate(75, (uint64_t)isr75);
  SetIDTGate(76, (uint64_t)isr76);
  SetIDTGate(77, (uint64_t)isr77);
  SetIDTGate(78, (uint64_t)isr78);
  SetIDTGate(79, (uint64_t)isr79);
  SetIDTGate(80, (uint64_t)isr80);
  SetIDTGate(81, (uint64_t)isr81);
  SetIDTGate(82, (uint64_t)isr82);
  SetIDTGate(83, (uint64_t)isr83);
  SetIDTGate(84, (uint64_t)isr84);
  SetIDTGate(85, (uint64_t)isr85);
  SetIDTGate(86, (uint64_t)isr86);
  SetIDTGate(87, (uint64_t)isr87);
  SetIDTGate(88, (uint64_t)isr88);
  SetIDTGate(89, (uint64_t)isr89);
  SetIDTGate(90, (uint64_t)isr90);
  SetIDTGate(91, (uint64_t)isr91);
  SetIDTGate(92, (uint64_t)isr92);
  SetIDTGate(93, (uint64_t)isr93);
  SetIDTGate(94, (uint64_t)isr94);
  SetIDTGate(95, (uint64_t)isr95);
  SetIDTGate(96, (uint64_t)isr96);
  SetIDTGate(97, (uint64_t)isr97);
  SetIDTGate(98, (uint64_t)isr98);
  SetIDTGate(99, (uint64_t)isr99);
  SetIDTGate(100, (uint64_t)isr100);
  SetIDTGate(101, (uint64_t)isr101);
  SetIDTGate(102, (uint64_t)isr102);
  SetIDTGate(103, (uint64_t)isr103);
  SetIDTGate(104, (uint64_t)isr104);
  SetIDTGate(105, (uint64_t)isr105);
  SetIDTGate(106, (uint64_t)isr106);
  SetIDTGate(107, (uint64_t)isr107);
  SetIDTGate(108, (uint64_t)isr108);
  SetIDTGate(109, (uint64_t)isr109);
  SetIDTGate(110, (uint64_t)isr110);
  SetIDTGate(111, (uint64_t)isr111);
  SetIDTGate(112, (uint64_t)isr112);
  SetIDTGate(113, (uint64_t)isr113);
  SetIDTGate(114, (uint64_t)isr114);
  SetIDTGate(115, (uint64_t)isr115);
  SetIDTGate(116, (uint64_t)isr116);
  SetIDTGate(117, (uint64_t)isr117);
  SetIDTGate(118, (uint64_t)isr118);
  SetIDTGate(119, (uint64_t)isr119);
  SetIDTGate(120, (uint64_t)isr120);
  SetIDTGate(121, (uint64_t)isr121);
  SetIDTGate(122, (uint64_t)isr122);
  SetIDTGate(123, (uint64_t)isr123);
  SetIDTGate(124, (uint64_t)isr124);
  SetIDTGate(125, (uint64_t)isr125);
  SetIDTGate(126, (uint64_t)isr126);
  SetIDTGate(127, (uint64_t)isr127);
  SetIDTGate(128, (uint64_t)isr128);
  SetIDTGate(129, (uint64_t)isr129);
  SetIDTGate(130, (uint64_t)isr130);
  SetIDTGate(131, (uint64_t)isr131);
  SetIDTGate(132, (uint64_t)isr132);
  SetIDTGate(133, (uint64_t)isr133);
  SetIDTGate(134, (uint64_t)isr134);
  SetIDTGate(135, (uint64_t)isr135);
  SetIDTGate(136, (uint64_t)isr136);
  SetIDTGate(137, (uint64_t)isr137);
  SetIDTGate(138, (uint64_t)isr138);
  SetIDTGate(139, (uint64_t)isr139);
  SetIDTGate(140, (uint64_t)isr140);
  SetIDTGate(141, (uint64_t)isr141);
  SetIDTGate(142, (uint64_t)isr142);
  SetIDTGate(143, (uint64_t)isr143);
  SetIDTGate(144, (uint64_t)isr144);
  SetIDTGate(145, (uint64_t)isr145);
  SetIDTGate(146, (uint64_t)isr146);
  SetIDTGate(147, (uint64_t)isr147);
  SetIDTGate(148, (uint64_t)isr148);
  SetIDTGate(149, (uint64_t)isr149);
  SetIDTGate(150, (uint64_t)isr150);
  SetIDTGate(151, (uint64_t)isr151);
  SetIDTGate(152, (uint64_t)isr152);
  SetIDTGate(153, (uint64_t)isr153);
  SetIDTGate(154, (uint64_t)isr154);
  SetIDTGate(155, (uint64_t)isr155);
  SetIDTGate(156, (uint64_t)isr156);
  SetIDTGate(157, (uint64_t)isr157);
  SetIDTGate(158, (uint64_t)isr158);
  SetIDTGate(159, (uint64_t)isr159);
  SetIDTGate(160, (uint64_t)isr160);
  SetIDTGate(161, (uint64_t)isr161);
  SetIDTGate(162, (uint64_t)isr162);
  SetIDTGate(163, (uint64_t)isr163);
  SetIDTGate(164, (uint64_t)isr164);
  SetIDTGate(165, (uint64_t)isr165);
  SetIDTGate(166, (uint64_t)isr166);
  SetIDTGate(167, (uint64_t)isr167);
  SetIDTGate(168, (uint64_t)isr168);
  SetIDTGate(169, (uint64_t)isr169);
  SetIDTGate(170, (uint64_t)isr170);
  SetIDTGate(171, (uint64_t)isr171);
  SetIDTGate(172, (uint64_t)isr172);
  SetIDTGate(173, (uint64_t)isr173);
  SetIDTGate(174, (uint64_t)isr174);
  SetIDTGate(175, (uint64_t)isr175);
  SetIDTGate(176, (uint64_t)isr176);
  SetIDTGate(177, (uint64_t)isr177);
  SetIDTGate(178, (uint64_t)isr178);
  SetIDTGate(179, (uint64_t)isr179);
  SetIDTGate(180, (uint64_t)isr180);
  SetIDTGate(181, (uint64_t)isr181);
  SetIDTGate(182, (uint64_t)isr182);
  SetIDTGate(183, (uint64_t)isr183);
  SetIDTGate(184, (uint64_t)isr184);
  SetIDTGate(185, (uint64_t)isr185);
  SetIDTGate(186, (uint64_t)isr186);
  SetIDTGate(187, (uint64_t)isr187);
  SetIDTGate(188, (uint64_t)isr188);
  SetIDTGate(189, (uint64_t)isr189);
  SetIDTGate(190, (uint64_t)isr190);
  SetIDTGate(191, (uint64_t)isr191);
  SetIDTGate(192, (uint64_t)isr192);
  SetIDTGate(193, (uint64_t)isr193);
  SetIDTGate(194, (uint64_t)isr194);
  SetIDTGate(195, (uint64_t)isr195);
  SetIDTGate(196, (uint64_t)isr196);
  SetIDTGate(197, (uint64_t)isr197);
  SetIDTGate(198, (uint64_t)isr198);
  SetIDTGate(199, (uint64_t)isr199);
  SetIDTGate(200, (uint64_t)isr200);
  SetIDTGate(201, (uint64_t)isr201);
  SetIDTGate(202, (uint64_t)isr202);
  SetIDTGate(203, (uint64_t)isr203);
  SetIDTGate(204, (uint64_t)isr204);
  SetIDTGate(205, (uint64_t)isr205);
  SetIDTGate(206, (uint64_t)isr206);
  SetIDTGate(207, (uint64_t)isr207);
  SetIDTGate(208, (uint64_t)isr208);
  SetIDTGate(209, (uint64_t)isr209);
  SetIDTGate(210, (uint64_t)isr210);
  SetIDTGate(211, (uint64_t)isr211);
  SetIDTGate(212, (uint64_t)isr212);
  SetIDTGate(213, (uint64_t)isr213);
  SetIDTGate(214, (uint64_t)isr214);
  SetIDTGate(215, (uint64_t)isr215);
  SetIDTGate(216, (uint64_t)isr216);
  SetIDTGate(217, (uint64_t)isr217);
  SetIDTGate(218, (uint64_t)isr218);
  SetIDTGate(219, (uint64_t)isr219);
  SetIDTGate(220, (uint64_t)isr220);
  SetIDTGate(221, (uint64_t)isr221);
  SetIDTGate(222, (uint64_t)isr222);
  SetIDTGate(223, (uint64_t)isr223);
  SetIDTGate(224, (uint64_t)isr224);
  SetIDTGate(225, (uint64_t)isr225);
  SetIDTGate(226, (uint64_t)isr226);
  SetIDTGate(227, (uint64_t)isr227);
  SetIDTGate(228, (uint64_t)isr228);
  SetIDTGate(229, (uint64_t)isr229);
  SetIDTGate(230, (uint64_t)isr230);
  SetIDTGate(231, (uint64_t)isr231);
  SetIDTGate(232, (uint64_t)isr232);
  SetIDTGate(233, (uint64_t)isr233);
  SetIDTGate(234, (uint64_t)isr234);
  SetIDTGate(235, (uint64_t)isr235);
  SetIDTGate(236, (uint64_t)isr236);
  SetIDTGate(237, (uint64_t)isr237);
  SetIDTGate(238, (uint64_t)isr238);
  SetIDTGate(239, (uint64_t)isr239);
  SetIDTGate(240, (uint64_t)isr240);
  SetIDTGate(241, (uint64_t)isr241);
  SetIDTGate(242, (uint64_t)isr242);
  SetIDTGate(243, (uint64_t)isr243);
  SetIDTGate(244, (uint64_t)isr244);
  SetIDTGate(245, (uint64_t)isr245);
  SetIDTGate(246, (uint64_t)isr246);
  SetIDTGate(247, (uint64_t)isr247);
  SetIDTGate(248, (uint64_t)isr248);
  SetIDTGate(249, (uint64_t)isr249);
  SetIDTGate(250, (uint64_t)isr250);
  SetIDTGate(251, (uint64_t)isr251);
  SetIDTGate(252, (uint64_t)isr252);
  SetIDTGate(253, (uint64_t)isr253);
  SetIDTGate(254, (uint64_t)isr254);
  SetIDTGate(255, (uint64_t)isr255);
  lprintf(logging_level::OK, "ISR initialized.\n");
 
}

typedef struct {
    uint8_t instructionnum;
    const char* mnemonic;  // Instruction mnemonic
    const char* description;  // Description of the instruction
    const char* operands;  // Operand format (e.g., "REG, IMM" or "REG, REG")
} instruction_t;

instruction_t instruction_conversion[256] = {
    { 0x00, "ADD", "Add with carry", "REG, REG/MEM" },
    { 0x01, "ADD", "Add with carry", "MEM/REG, REG" },
    { 0x02, "ADD", "Add with carry", "REG, MEM/REG" },
    { 0x03, "ADD", "Add with carry", "REG, REG" },
    { 0x04, "ADD", "Add with carry", "AL, IMM" },
    { 0x05, "ADD", "Add with carry", "AX, IMM" },
    { 0x06, "PUSH", "Push processor flags onto stack", "" },
    { 0x07, "POP", "Pop processor flags from stack", "" },
    { 0x08, "OR", "Logical OR", "REG, REG/MEM" },
    { 0x09, "OR", "Logical OR", "MEM/REG, REG" },
    { 0x0A, "OR", "Logical OR", "REG, MEM/REG" },
    { 0x0B, "OR", "Logical OR", "REG, REG" },
    { 0x0C, "OR", "Logical OR", "AL, IMM" },
    { 0x0D, "OR", "Logical OR", "AX, IMM" },
    { 0x0E, "PUSH", "Push CS register", "" },
    { 0x0F, "MOV", "Move between registers", "REG, REG/MEM" },
    { 0x10, "ADC", "Add with carry", "REG, REG/MEM" },
    { 0x11, "ADC", "Add with carry", "MEM/REG, REG" },
    { 0x12, "ADC", "Add with carry", "REG, MEM/REG" },
    { 0x13, "ADC", "Add with carry", "REG, REG" },
    { 0x14, "ADC", "Add with carry", "AL, IMM" },
    { 0x15, "ADC", "Add with carry", "AX, IMM" },
    { 0x16, "PUSH", "Push SS register", "" },
    { 0x17, "POP", "Pop SS register", "" },
    { 0x18, "SBB", "Subtract with borrow", "REG, REG/MEM" },
    { 0x19, "SBB", "Subtract with borrow", "MEM/REG, REG" },
    { 0x1A, "SBB", "Subtract with borrow", "REG, MEM/REG" },
    { 0x1B, "SBB", "Subtract with borrow", "REG, REG" },
    { 0x1C, "SBB", "Subtract with borrow", "AL, IMM" },
    { 0x1D, "SBB", "Subtract with borrow", "AX, IMM" },
    { 0x1E, "PUSH", "Push DS register", "" },
    { 0x1F, "POP", "Pop DS register", "" },
    { 0x20, "AND", "Logical AND", "REG, REG/MEM" },
    { 0x21, "AND", "Logical AND", "MEM/REG, REG" },
    { 0x22, "AND", "Logical AND", "REG, MEM/REG" },
    { 0x23, "AND", "Logical AND", "REG, REG" },
    { 0x24, "AND", "Logical AND", "AL, IMM" },
    { 0x25, "AND", "Logical AND", "AX, IMM" },
    { 0x26, "ES:", "Override to segment ES", "" },
    { 0x27, "DAA", "Decimal adjust AL after addition", "" },
    { 0x28, "SUB", "Subtract", "REG, REG/MEM" },
    { 0x29, "SUB", "Subtract", "MEM/REG, REG" },
    { 0x2A, "SUB", "Subtract", "REG, MEM/REG" },
    { 0x2B, "SUB", "Subtract", "REG, REG" },
    { 0x2C, "SUB", "Subtract", "AL, IMM" },
    { 0x2D, "SUB", "Subtract", "AX, IMM" },
    { 0x2E, "CS:", "Override to segment CS", "" },
    { 0x2F, "DAS", "Decimal adjust AL after subtraction", "" },
    { 0x30, "XOR", "Logical XOR", "REG, REG/MEM" },
    { 0x31, "XOR", "Logical XOR", "MEM/REG, REG" },
    { 0x32, "XOR", "Logical XOR", "REG, MEM/REG" },
    { 0x33, "XOR", "Logical XOR", "REG, REG" },
    { 0x34, "XOR", "Logical XOR", "AL, IMM" },
    { 0x35, "XOR", "Logical XOR", "AX, IMM" },
    { 0x36, "SS:", "Override to segment SS", "" },
    { 0x37, "AAA", "ASCII adjust AL after addition", "" },
    { 0x38, "CMP", "Compare", "REG, REG/MEM" },
    { 0x39, "CMP", "Compare", "MEM/REG, REG" },
    { 0x3A, "CMP", "Compare", "REG, MEM/REG" },
    { 0x3B, "CMP", "Compare", "REG, REG" },
    { 0x3C, "CMP", "Compare", "AL, IMM" },
    { 0x3D, "CMP", "Compare", "AX, IMM" },
    { 0x3E, "DS:", "Override to segment DS", "" },
    { 0x3F, "AAS", "ASCII adjust AX after subtraction", "" },
    { 0x40, "INC", "Increment register by 1", "REG" },
    { 0x41, "INC", "Increment register by 1", "REG" },
    { 0x42, "INC", "Increment register by 1", "REG" },
    { 0x43, "INC", "Increment register by 1", "REG" },
    { 0x44, "INC", "Increment register by 1", "REG" },
    { 0x45, "INC", "Increment register by 1", "REG" },
    { 0x46, "INC", "Increment register by 1", "REG" },
    { 0x47, "INC", "Increment register by 1", "REG" },
    { 0x48, "DEC", "Decrement register by 1", "REG" },
    { 0x49, "DEC", "Decrement register by 1", "REG" },
    { 0x4A, "DEC", "Decrement register by 1", "REG" },
    { 0x4B, "DEC", "Decrement register by 1", "REG" },
    { 0x4C, "DEC", "Decrement register by 1", "REG" },
    { 0x4D, "DEC", "Decrement register by 1", "REG" },
    { 0x4E, "DEC", "Decrement register by 1", "REG" },
    { 0x4F, "DEC", "Decrement register by 1", "REG" },
    { 0x50, "PUSH", "Push general-purpose register", "AX" },
    { 0x51, "PUSH", "Push general-purpose register", "CX" },
    { 0x52, "PUSH", "Push general-purpose register", "DX" },
    { 0x53, "PUSH", "Push general-purpose register", "BX" },
    { 0x54, "PUSH", "Push general-purpose register", "SP" },
    { 0x55, "PUSH", "Push general-purpose register", "BP" },
    { 0x56, "PUSH", "Push general-purpose register", "SI" },
    { 0x57, "PUSH", "Push general-purpose register", "DI" },
    { 0x58, "POP", "Pop general-purpose register", "AX" },
    { 0x59, "POP", "Pop general-purpose register", "CX" },
    { 0x5A, "POP", "Pop general-purpose register", "DX" },
    { 0x5B, "POP", "Pop general-purpose register", "BX" },
    { 0x5C, "POP", "Pop general-purpose register", "SP" },
    { 0x5D, "POP", "Pop general-purpose register", "BP" },
    { 0x5E, "POP", "Pop general-purpose register", "SI" },
    { 0x5F, "POP", "Pop general-purpose register", "DI" },
    { 0x60, "PUSHAD", "Push all general-purpose registers", "" },
    { 0x61, "POPAD", "Pop all general-purpose registers", "" },
    { 0x62, "BND", "Bound check", "REG, MEM" },
    { 0x63, "ARPL", "Adjust RPL field", "MEM/REG, REG" },
    { 0x64, "FS:", "Override to segment FS", "" },
    { 0x65, "GS:", "Override to segment GS", "" },
    { 0x66, "Operand size override", "Overrides the operand size", "" },
    { 0x67, "Address size override", "Overrides the address size", "" },
    { 0x68, "PUSH", "Push immediate value", "IMM" },
    { 0x69, "IMUL", "Signed multiply", "REG, REG/MEM, IMM" },
    { 0x6A, "PUSH", "Push immediate byte", "IMM8" },
    { 0x6B, "IMUL", "Signed multiply", "REG, REG/MEM, IMM8" },
    { 0x6C, "INSB", "Input from port to AL", "" },
    { 0x6D, "INSW", "Input from port to AX", "" },
    { 0x6E, "OUTSB", "Output from AL to port", "" },
    { 0x6F, "OUTSW", "Output from AX to port", "" },
    { 0x70, "JO", "Jump if overflow", "REL" },
    { 0x71, "JNO", "Jump if no overflow", "REL" },
    { 0x72, "JC", "Jump if carry", "REL" },
    { 0x73, "JNC", "Jump if no carry", "REL" },
    { 0x74, "JZ", "Jump if zero", "REL" },
    { 0x75, "JNZ", "Jump if not zero", "REL" },
    { 0x76, "JA", "Jump if above", "REL" },
    { 0x77, "JNA", "Jump if not above", "REL" },
    { 0x78, "JS", "Jump if sign", "REL" },
    { 0x79, "JNS", "Jump if no sign", "REL" },
    { 0x7A, "JP", "Jump if parity", "REL" },
    { 0x7B, "JNP", "Jump if no parity", "REL" },
    { 0x7C, "JL", "Jump if less", "REL" },
    { 0x7D, "JNL", "Jump if not less", "REL" },
    { 0x7E, "JLE", "Jump if less or equal", "REL" },
    { 0x7F, "JNLE", "Jump if not less or equal", "REL" },
    { 0x80, "JO", "Jump if overflow", "REL" },
    { 0x81, "JNO", "Jump if no overflow", "REL" },
    { 0x82, "JC", "Jump if carry", "REL" },
    { 0x83, "JNC", "Jump if no carry", "REL" },
    { 0x84, "JZ", "Jump if zero", "REL" },
    { 0x85, "JNZ", "Jump if not zero", "REL" },
    { 0x86, "JA", "Jump if above", "REL" },
    { 0x87, "JNA", "Jump if not above", "REL" },
    { 0x88, "MOV", "Move between registers/memory", "REG, MEM/REG" },
    { 0x89, "MOV", "Move between registers/memory", "MEM/REG, REG" },
    { 0x8A, "MOV", "Move byte between registers/memory", "REG, MEM/REG" },
    { 0x8B, "MOV", "Move word between registers/memory", "REG, MEM/REG" },
    { 0x8C, "MOV", "Move to a segment register", "REG, SEG" },
    { 0x8D, "LEA", "Load effective address", "REG, MEM/REG" },
    { 0x8E, "MOV", "Move from segment register", "SEG, REG" },
    { 0x8F, "POP", "Pop from memory to register", "MEM/REG" },
    { 0x90, "NOP", "No operation", "" },
    { 0x91, "XCHG", "Exchange register with AX", "REG" },
    { 0x92, "XCHG", "Exchange register with DX", "REG" },
    { 0x93, "XCHG", "Exchange register with CX", "REG" },
    { 0x94, "XCHG", "Exchange register with BX", "REG" },
    { 0x95, "XCHG", "Exchange register with SP", "REG" },
    { 0x96, "XCHG", "Exchange register with BP", "REG" },
    { 0x97, "XCHG", "Exchange register with SI", "REG" },
    { 0x98, "CBW", "Convert byte to word", "" },
    { 0x99, "CWD", "Convert word to doubleword", "" },
    { 0x9A, "CALL", "Call procedure", "IMM16" },
    { 0x9B, "WAIT", "Wait for an event (e.g., for a coprocessor)", "" },
    { 0x9C, "PUSHF", "Push flags onto stack", "" },
    { 0x9D, "POPF", "Pop flags from stack", "" },
    { 0x9E, "SAHF", "Store AH into flags", "" },
    { 0x9F, "LAHF", "Load flags into AH", "" },
    { 0xA0, "MOV", "Move byte from memory to AL", "MEM" },
    { 0xA1, "MOV", "Move word from memory to AX", "MEM" },
    { 0xA2, "MOV", "Move byte from AL to memory", "MEM" },
    { 0xA3, "MOV", "Move word from AX to memory", "MEM" },
    { 0xA4, "MOVSB", "Move byte from DS:SI to ES:DI", "" },
    { 0xA5, "MOVSW", "Move word from DS:SI to ES:DI", "" },
    { 0xA6, "CMPSB", "Compare byte in DS:SI with byte in ES:DI", "" },
    { 0xA7, "CMPSW", "Compare word in DS:SI with word in ES:DI", "" },
    { 0xA8, "TEST", "Test AL with immediate", "AL, IMM" },
    { 0xA9, "TEST", "Test AX with immediate", "AX, IMM" },
    { 0xAA, "STOSB", "Store byte from AL to ES:DI", "" },
    { 0xAB, "STOSW", "Store word from AX to ES:DI", "" },
    { 0xAC, "LODSB", "Load byte from DS:SI into AL", "" },
    { 0xAD, "LODSW", "Load word from DS:SI into AX", "" },
    { 0xAE, "SCASB", "Scan byte in AL against byte at ES:DI", "" },
    { 0xAF, "SCASW", "Scan word in AX against word at ES:DI", "" },
    { 0xB0, "MOV", "Move immediate byte to AL", "IMM8" },
    { 0xB1, "MOV", "Move immediate byte to CL", "IMM8" },
    { 0xB2, "MOV", "Move immediate byte to DL", "IMM8" },
    { 0xB3, "MOV", "Move immediate byte to BL", "IMM8" },
    { 0xB4, "MOV", "Move immediate byte to AH", "IMM8" },
    { 0xB5, "MOV", "Move immediate byte to CH", "IMM8" },
    { 0xB6, "MOV", "Move immediate byte to DH", "IMM8" },
    { 0xB7, "MOV", "Move immediate byte to BH", "IMM8" },
    { 0xB8, "MOV", "Move immediate word to AX", "IMM16" },
    { 0xB9, "MOV", "Move immediate word to CX", "IMM16" },
    { 0xBA, "MOV", "Move immediate word to DX", "IMM16" },
    { 0xBB, "MOV", "Move immediate word to BX", "IMM16" },
    { 0xBC, "MOV", "Move immediate word to SP", "IMM16" },
    { 0xBD, "MOV", "Move immediate word to BP", "IMM16" },
    { 0xBE, "MOV", "Move immediate word to SI", "IMM16" },
    { 0xBF, "MOV", "Move immediate word to DI", "IMM16" },
    { 0xC0, "RET", "Return from procedure", "IMM16" },
    { 0xC1, "RET", "Return from procedure", "IMM8" },
    { 0xC2, "RETF", "Return far from procedure", "IMM16" },
    { 0xC3, "RETF", "Return far from procedure", "" },
    { 0xC4, "IN", "Input from port to AL", "IMM8" },
    { 0xC5, "IN", "Input from port to AX", "IMM8" },
    { 0xC6, "OUT", "Output from AL to port", "IMM8" },
    { 0xC7, "OUT", "Output from AX to port", "IMM8" },
    { 0xC8, "PUSH", "Push word onto stack", "IMM16" },
    { 0xC9, "POP", "Pop word from stack", "IMM16" },
    { 0xCA, "BOUND", "Bound check", "REG, MEM" },
    { 0xCB, "ARPL", "Adjust RPL field", "REG, REG" },
    { 0xCC, "INT3", "Software interrupt", "" },
    { 0xCD, "INT", "Software interrupt", "IMM8" },
    { 0xCE, "INTO", "Interrupt on overflow", "" },
    { 0xCF, "IRET", "Return from interrupt", "" },
    { 0xD0, "ROL", "Rotate left", "REG/MEM" },
    { 0xD1, "ROR", "Rotate right", "REG/MEM" },
    { 0xD2, "RCL", "Rotate through carry left", "REG/MEM" },
    { 0xD3, "RCR", "Rotate through carry right", "REG/MEM" },
    { 0xD4, "SHL", "Shift left", "REG/MEM" },
    { 0xD5, "SHR", "Shift right", "REG/MEM" },
    { 0xD6, "SAL", "Shift arithmetic left", "REG/MEM" },
    { 0xD7, "SAR", "Shift arithmetic right", "REG/MEM" },
    { 0xD8, "FADD", "Floating-point add", "ST, ST(0)" },
    { 0xD9, "FSUB", "Floating-point subtract", "ST, ST(0)" },
    { 0xDA, "FMUL", "Floating-point multiply", "ST, ST(0)" },
    { 0xDB, "FDIV", "Floating-point divide", "ST, ST(0)" },
    { 0xDC, "FST", "Floating-point store", "ST, MEM" },
    { 0xDD, "FSTP", "Floating-point store and pop", "ST, MEM" },
    { 0xDE, "FLD", "Floating-point load", "MEM, ST" },
    { 0xDF, "FLDENV", "Load floating-point environment", "MEM" },
    { 0xE0, "LOOPNE", "Loop while CX != 0 and ZF = 0", "IMM8" },
    { 0xE1, "LOOPE", "Loop while CX != 0 and ZF = 1", "IMM8" },
    { 0xE2, "LOOP", "Loop while CX != 0", "IMM8" },
    { 0xE3, "JECXZ", "Jump if ECX = 0", "IMM8" },
    { 0xE4, "IN", "Input from port to AL", "IMM8" },
    { 0xE5, "IN", "Input from port to AX", "IMM8" },
    { 0xE6, "OUT", "Output from AL to port", "IMM8" },
    { 0xE7, "OUT", "Output from AX to port", "IMM8" },
    { 0xE8, "CALL", "Call near procedure", "IMM16" },
    { 0xE9, "JMP", "Jump near", "IMM16" },
    { 0xEA, "JMP", "Jump far", "IMM16" },
    { 0xEB, "JMP", "Jump short", "IMM8" },
    { 0xEC, "IN", "Input from port to AL", "DX" },
    { 0xED, "IN", "Input from port to AX", "DX" },
    { 0xEE, "OUT", "Output from AL to port", "DX" },
    { 0xEF, "OUT", "Output from AX to port", "DX" },
    { 0xF0, "LOCK", "Lock the bus for atomic operations", "" },
    { 0xF1, "INT1", "Software interrupt", "" },
    { 0xF2, "REPNE", "Repeat while ZF = 0", "" },
    { 0xF3, "REPE", "Repeat while ZF = 1", "" },
    { 0xF4, "HLT", "Halt execution", "" },
    { 0xF5, "CMC", "Complement carry flag", "" },
    { 0xF6, "TEST", "Test byte with immediate", "MEM, IMM8" },
    { 0xF7, "TEST", "Test word with immediate", "MEM, IMM16" },
    { 0xF8, "CLC", "Clear carry flag", "" },
    { 0xF9, "STC", "Set carry flag", "" },
    { 0xFA, "CLI", "Clear interrupt flag", "" },
    { 0xFB, "STI", "Set interrupt flag", "" },
    { 0xFC, "CLD", "Clear direction flag", "" },
    { 0xFD, "STD", "Set direction flag", "" },
    { 0xFE, "INC", "Increment byte/word at memory", "MEM" },
    { 0xFF, "INC", "Increment register", "REG" }
};

extern "C" void ISRHandler(registers_t *r) {
    if (r->isrNumber < 32) {
        uint64_t cr2;
        asm volatile(
            "cli\n" // Disable interrupts
            "mov %%cr2, %0"
            : "=a"(cr2));

        printf("\n");

        // Turn r->rip into a pointer and get the instruction and operands
        uint8_t instruction = *(uint8_t *)(r->rip - 2);

        // Print out exception information
        lprintf(logging_level::ERROR, "Exception [%s] (%d) at %l\n", exceptionMessages[r->isrNumber], r->isrNumber, r->rip);

        // Retrieve instruction mnemonic and operand format
        instruction_t *inst = &instruction_conversion[instruction];

        // Log the instruction mnemonic
        lprintf(logging_level::ERROR, "Instruction: %s\n", inst->mnemonic);
        for (;;) {
            asm volatile("cli");
            asm volatile("hlt");
        }
    }

    // After every interrupt, send an EOI to the PICs or they will not send another interrupt again
    outb(0xA0, 0x20);
    outb(0x20, 0x20);

    // Call the event handler if defined
    if (eventHandlers[r->isrNumber] != NULL) {
        eventHandlers[r->isrNumber](r);
    }

    // If no event handler is defined for this interrupt
    if (eventHandlers[r->isrNumber] == NULL) {
        
        return;
    }
}


