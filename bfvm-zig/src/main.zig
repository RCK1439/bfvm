const std = @import("std");

const Student = struct {
    name: []const u8,

    pub fn init() Student {
        return Student{ .name = undefined };
    }

    pub fn setName(self: *Student, name: []const u8) void {
        self.name = name;
    }
};

pub fn main() !void {
    var s = Student.init();
    s.setName("foo");

    std.debug.print("Student name: {s}\n", .{s.name});
}
