#define true (1 == 1)
#define false (!true)

// the 0 at the end is to enable the use of a semicolon at the end
#define letcheck(name, expr) let name = (expr); if (name) == null { return null; } 0
#define assigncheck(name, expr) name = (expr); if (name) == null { return null; } 0
