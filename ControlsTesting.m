clear all; close all; instrreset;
crpm = ControlsRPMLab("COM9")
dir = 1;
% This loop ramps motor up and down, then changes direction.
% Prints out current RPM at each setting.
while true 
    dir = double(~dir)'
    crpm.setDir(char(dir));
    for pwmVal = 0:80:255
        crpm.setPWM(pwmVal);
        pause(1);
        fprintf("RPM currently: %i\n", crpm.getRPM());
        pause(1);
    end
    for pwmVal = 255:-80:0
        crpm.setPWM(pwmVal);
        pause(1);
        fprintf("RPM currently: %i\n", crpm.getRPM());
        pause(1);
    end
end