classdef ControlsRPMLab < handle
    % Matlab class for Controls RPM Lab - Allows for PWM and Dir control 
    % of equipment. Can request RPM values back too.
    %
    % Usage example:
    % rpmObj = ControlsRPMLab("COM3");
    % rpmObj.setDir(1); % Sets a direction value of 1
    % rpmObj.setPWM(127); % Sets a PWM value of 127
    % whatiscurrentRPM = rpmObj.getRPM() % Gets current RPM reading
    
    properties (Access = private)
        sPort
    end 
    
    methods
        function obj = ControlsRPMLab(aPort)
            obj.sPort = serial(aPort, 'BaudRate', 115200);
            fopen(obj.sPort); 
        end
        
        function setPWM(obj,pwmValue) 
            % setPWM(rpmObj, pwmValue) - Sends pwmValue on 0-255 range to
            % microcontroller
            pwmValue = max(0, min(pwmValue,255));
            flushinput(obj.sPort); 
            fprintf(obj.sPort, "p" + uint8(pwmValue));
        end
        
        function setDir(obj,dirValue) 
            % setDir(rpmObj, dirValue) - Sends dirValue (0 or 1) to microcontroller
            pwmValue = round(max(0, min(dirValue,1)));
            flushinput(obj.sPort); 
            fprintf(obj.sPort, "d" + uint8(dirValue));
        end
        
        function rpmValue = getRPM(obj) 
            % rpmValue = getRPM(rpmObj) - Requests current RPM reading from
            % microcontroller and returns it to Matlab
            flushinput(obj.sPort); 
            fprintf(obj.sPort, "r" );
            serialTimer = tic;
            data = "";
            while(toc(serialTimer) < 15)  
                out = get(obj.sPort, 'BytesAvailable'); 
                if (out > 0)  
                    data = data + fscanf(obj.sPort); 
                else 
                    pause(0.01);
                end
                if contains(data, newline) || contains(data, char(13))
                    rpmValue = str2double(replace(replace(data,'\r',''),'\n','')); 
                    return;
                end
            end
            error("Never received RPM data back from microcontroller");
        end
    end
end

