function client()
%   provides a menu for accessing PIC32 motor control functions
%
%   client(port)
%
%   Input Arguments:
%       port - the name of the com port.  This should be the same as what
%               you use in screen or putty in quotes ' '
%
%   Example:
%       client('/dev/ttyUSB0') (Linux/Mac)
%       client('COM3') (PC)
%
%   For convenience, you may want to change this so that the port is hardcoded.
   
% Opening COM connection
if ~isempty(instrfind)
    fclose(instrfind);
    delete(instrfind);
end

port = '/dev/ttyUSB0';

fprintf('Opening port %s....\n',port);

% settings for opening the serial port. baud rate 230400, hardware flow control
% wait up to 120 seconds for data before timing out
mySerial = serial(port, 'BaudRate', 230400, 'FlowControl', 'hardware','Timeout',120); 
% opens serial connection
fopen(mySerial);
% closes serial port when function exits
clean = onCleanup(@()fclose(mySerial));                                 

has_quit = false;
% menu loop
while ~has_quit
    fprintf('PIC32 MOTOR DRIVER INTERFACE\n\n');
    % display the menu options; this list will grow
    fprintf('     a: Read Current Sensor (ADC counts)    j: \n');
    fprintf('     b: Read Current Sensor (mA)            k: Test Current Control\n');
    fprintf('     c: Read Encoder (counts)               l: \n');
    fprintf('     d: Read Encoder (deg)                  m: \n');
    fprintf('     e: Reset Encoder                       n: \n');
    fprintf('     f: Set PWM (-100 to 100)               o: \n');
    fprintf('     g: Set Current Gains                   p: Unpower Motor\n');
    fprintf('     h: Get Current Gains                   q: Quit\n');
    fprintf('     i:                                     r: Get Mode\n');
    % read the user's choice
    selection = input('\nENTER COMMAND: ', 's');
     
    % send the command to the PIC32
    fprintf(mySerial,'%c\n',selection);
    
    % take the appropriate action
    switch selection
        case 'a'
            adc_cnt = fscanf(mySerial, '%d');
            fprintf('Current Sensor (in counts): %d\n\n', adc_cnt);
            
        case 'b'
            adc_ma = fscanf(mySerial, '%d');
            fprintf('Current Sensor (in mA): %d\n\n', adc_ma);
            
        case 'c'
            cnt = fscanf(mySerial,'%d');   % get the incremented number back
            fprintf('Encoder count: %d\n\n',cnt);     % print it to the screen
            
        case 'd'
            deg = fscanf(mySerial,'%d');   % get the incremented number back
            fprintf('Rotation: %d degrees\n\n',deg);     % print it to the screen
            
        case 'e'
            fscanf(mySerial, '%d');
            fprintf('Position reset!\n\n');
            
        case 'f'
            speed = input('Enter speed: ');
            
            if (speed <= 100) && (speed >= -100)
                fprintf(mySerial, '%d\n', speed);
                fprintf('Speed set to %d\n\n', speed);
            else
                fprintf('Invalid speed: %d\n\n', speed);
            end
            
        case 'g'
            kp = input('Enter Kp: ');
            fprintf(mySerial, '%d\n', kp);
            ki = input('Enter Ki: ');
            fprintf(mySerial, '%d\n', ki);
            fprintf('Gains set!\n\n');
        
        case 'h'
            kp = fscanf(mySerial, '%d');
            ki = fscanf(mySerial, '%d');
            fprintf('Kp: %d    Ki: %d', kp, ki);
            
        case 'p'
            fprintf('Motor unpowered.\n');
            fprintf('Mode set to IDLE\n\n');
            
        case 'q'
            fprintf('Mode set to IDLE\n');
            fprintf("We're done here.\n\n");
            has_quit = true;             % exit client
            
        case 'r'
            m = fscanf(mySerial, '%d');
            fprintf('Mode: %d\n\n', m);
            
        otherwise
            fprintf('Invalid Selection %c\n\n', selection);
    end
end

end
