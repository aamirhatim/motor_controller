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
    fprintf('     a: Read Current Sensor (ADC counts)    j: Get Position Gains\n');
    fprintf('     b: Read Current Sensor (mA)            k: Test Current Control\n');
    fprintf('     c: Read Encoder (counts)               l: Go to Angle (deg)\n');
    fprintf('     d: Read Encoder (deg)                  m: Load Step Trajectory\n');
    fprintf('     e: Reset Encoder                       n: Load Cubic Trajectory\n');
    fprintf('     f: Set PWM (-100 to 100)               o: Execute Trajectory\n');
    fprintf('     g: Set Current Gains                   p: Unpower Motor\n');
    fprintf('     h: Get Current Gains                   q: Quit\n');
    fprintf('     i: Set Position Gains                  r: Get Mode\n');
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
            adc_ma = fscanf(mySerial, '%f');
            fprintf('Current Sensor (in mA): %f\n\n', round(adc_ma,2));
            
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
            kpset = input('Enter Kp: ');
            fprintf(mySerial, '%f\n', kpset);
            kiset = input('Enter Ki: ');
            fprintf(mySerial, '%f\n', kiset);
            fprintf('Current Gains set!\n\n');
        
        case 'h'
            kpget = fscanf(mySerial, '%f');
            kiget = fscanf(mySerial, '%f');
            fprintf('Kp: %f    Ki: %f\n\n', kpget, kiget);
            
        case 'i'
            kpset = input('Enter Kp: ');
            fprintf(mySerial, '%f\n', kpset);
            kiset = input('Enter Ki: ');
            fprintf(mySerial, '%f\n', kiset);
            kdset = input('Enter Kd: ');
            fprintf(mySerial, '%f\n', kdset);
            fprintf('Position Gains set!\n\n');
            
        case 'j'
            kpget = fscanf(mySerial, '%f');
            kiget = fscanf(mySerial, '%f');
            kdget = fscanf(mySerial, '%f');
            fprintf('Kp: %f    Ki: %f    Kd: %f\n\n', kpget, kiget, kdget);
            
        case 'k'
            %% Reading Data
            fprintf("Waiting for samples...\n");

            data = zeros(100,2);
            
            for i = 1:100
                data(i,:) = fscanf(mySerial, '%d %d');
                times(i) = (i-1)*.2;
            end
            stairs(times, data(:,1:2));
            
            score = mean(abs(data(:,1)-data(:,2)));
            title(sprintf('\nAverage Error: %5.1f mA\n\n', score));
            
        case 'l'
            a = input('Enter desired angle (deg): ');
            fprintf(mySerial, '%d\n', a);
            fprintf('Moving to angle: %d\n\n', a);
            
        case 'm'
            trajectory = input('Enter your trajectory:\n');
            ref = genRef(trajectory, 'step');
            fprintf(mySerial, '%d\n', length(ref));
            fprintf('Sending trajectory...\n');
            for i = 1:length(ref)
                ref(i) = (ref(i)*1792/360) + 32768; %convert to ticks value
                fprintf(mySerial, '%d\n', ref(i));
            end
            
            fprintf('Trajectory sent!\n\n');
           
        case 'n'
            trajectory = input('Enter your trajectory:\n');
            ref = genRef(trajectory, 'cubic');
            fprintf(mySerial, '%d\n', length(ref));
            fprintf('Sending trajectory...\n');
            for i = 1:length(ref)
                ref(i) = (ref(i)*1792/360) + 32768; %convert to ticks value
                fprintf(mySerial, '%f\n', ref(i));
            end
            
            fprintf('Trajectory sent!\n\n');
            
        case 'o'
            %% Reading Data
            fprintf("Waiting for samples...\n");
            l = fscanf(mySerial, '%d');

            data = zeros(l,2);
            
            for i = 1:l
                data(i,:) = fscanf(mySerial, '%d %d');
                times(i) = (i-1)*.005;
            end
            stairs(times, data(:,1:2));
            
            score = mean(abs(data(:,1)-data(:,2)));
            title(sprintf('\nAverage Error: %5.1f degrees\n\n', score));
            xlabel('Time (s)');
            ylabel('Angle (degrees)');
            
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
