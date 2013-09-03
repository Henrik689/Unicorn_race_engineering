unit statmain;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, H8WinVars;



type
  TINJStat = class(TForm)
    GroupBox4: TGroupBox;
    Label9: TLabel;
    Label10: TLabel;
    Label11: TLabel;
    Label12: TLabel;
    Label13: TLabel;
    Label14: TLabel;
    Label15: TLabel;
    Label16: TLabel;
    Label17: TLabel;
    Label18: TLabel;
    Label20: TLabel;
    Label21: TLabel;
    Label22: TLabel;
    Label23: TLabel;
    Label24: TLabel;
    Label25: TLabel;
    Label26: TLabel;
    Label30: TLabel;
    WaterTempLabel: TLabel;
    AirTempLabel: TLabel;
    AirMassVoltLabel: TLabel;
    PotmeterLabel: TLabel;
    IdleLabel: TLabel;
    WideOpenTrottleLabel: TLabel;
    MapSensorLabel: TLabel;
    KL87VoltageLabel: TLabel;
    LambdaVoltageLabel: TLabel;
    RPMLabel: TLabel;
    ExhaustCamRawLabel: TLabel;
    KnockSensor1Label: TLabel;
    KnockSensor2Label: TLabel;
    RoadSpeedLabel: TLabel;
    WegFahrSperreLabel: TLabel;
    TractionLabel: TLabel;
    AntilagLabel: TLabel;
    KL15Label: TLabel;
    GroupBox2: TGroupBox;
    Label33: TLabel;
    Label34: TLabel;
    Label35: TLabel;
    Label36: TLabel;
    Label37: TLabel;
    Label39: TLabel;
    Label40: TLabel;
    Label41: TLabel;
    Label43: TLabel;
    Label44: TLabel;
    Label50: TLabel;
    Label51: TLabel;
    Label52: TLabel;
    Label54: TLabel;
    Label55: TLabel;
    Label57: TLabel;
    OTErrorlabel: TLabel;
    OTSynclabel: TLabel;
    IntakeVanosPWMLabel: TLabel;
    Loadlabel: TLabel;
    InjectorTimelabel: TLabel;
    IgnitionTimelabel: TLabel;
    Dwelllabel: TLabel;
    tisignallabel: TLabel;
    CamSynclabel: TLabel;
    ExhaustVanosPWMLabel: TLabel;
    pwmidlelabel: TLabel;
    Errorlamplabel: TLabel;
    mainrelaylabel: TLabel;
    fuelpumplabel: TLabel;
    evaporationlabel: TLabel;
    ChokerMultlabel: TLabel;
    label77: TLabel;
    LambdaZLabel: TLabel;
    Label1: TLabel;
    AirMassFlowLabel: TLabel;
    Label42: TLabel;
    UndrivLLabel: TLabel;
    Label2: TLabel;
    EgasFeedBackLabel: TLabel;
    Label3: TLabel;
    IntakeCamRawLabel: TLabel;
    Label19: TLabel;
    IntakeCamAdjLabel: TLabel;
    Label4: TLabel;
    ExhaustCamAdjLabel: TLabel;
    Label5: TLabel;
    ChokerAddLabel: TLabel;
    Label6: TLabel;
    UndrivRLabel: TLabel;
    Label7: TLabel;
    LambdaPowerPWMLabel: TLabel;
    Label8: TLabel;
    TractionRegulateLabel: TLabel;
    label111: TLabel;
    Label29: TLabel;
    LapCountLabel: TLabel;
    LapTimeLabel: TLabel;
    Simu1000: TButton;
    SimuOff: TButton;
    Simu100: TButton;
    Label27: TLabel;
    StatusTimeLabel: TLabel;
    Label28: TLabel;
    Label31: TLabel;
    Label32: TLabel;
    GXLabel: TLabel;
    GYLabel: TLabel;
    GZLabel: TLabel;
    Label38: TLabel;
    Powershift: TLabel;

    Procedure ShowStatus;
    procedure Simu1000Click(Sender: TObject);
    procedure SimuOffClick(Sender: TObject);
    procedure Simu100Click(Sender: TObject);

  private

    { Private declarations }
  public
    { Public declarations }
    Function  TempWrite(input:Word):string;
    Procedure AddLoggerData(ByteLen :integer);
  end;

var
  INJStat: TINJStat;

implementation

uses ComUnit;

Function TINJStat.TempWrite(input:Word):string;
var
  r    :real;

begin
  r := swap(input);
  r := 150 * r / 3840;
  r := 120 - r;
  str(r:3:1,Result);
  Result := Result + ' Gr C';
end;

Function SwapWrite(input:Word):string;
begin
  Result := Inttostr(swap(input));
end;

Function SpeedWrite(input:Word):String;
var
  r   :real;
begin
  r := swap(input);
  r := r / 100;
  str(r:5:1,Result);
  Result := Result + ' km/t';
end;

Function  SwapIntWrite(input:Integer):string;
var
  i   :Smallint;

begin
  input := swap(input);
  move(input,i,2);
  Result := IntToStr(i);
end;

Function  CamAngleWrite(input:SmallInt):string;
var
  i   :Smallint;
  r   :real;

begin
  input := swap(input);
  move(input,i,2);
  r := i * 0.75;
  str(r:3:1,Result);
  Result := Result + ' Deg';
end;

Function  RPMWrite(input:Word):string;
begin
  result := IntToStr(round(swap(input)*0.9408))+' RPM';
end;

Function Volt5Write(input:Word):string;
var
  r    :real;

begin
  r := swap(input);
  r := r / 800;
  str(r:4:2,Result);
  Result := Result + ' Volt';
end;

Function LambdaWrite(input:smallint):string;
var
  r    :real;

begin
  input := swap(input);
  r := input;

  begin
    if r < 0 then
    begin
      r := -r;
      r := 0.7+0.6*r/3840;
      r := r * Stoek_Metric;
      str(r:5:2,result);

      result := 'AFR '+result;
    end else
    begin
      r := r / 2400;
      {5V/3   = 1.6666V as max voltage}
      str(r:6:2,result);
      result := result+' Volt  ';
    end;
  end;
end;

Function Volt7Write(input:Word):string;
var
  r    :real;

begin
  r := swap(input);
  r := r / 686;
  str(r:4:2,Result);
  Result := Result + ' Volt';
end;

Function kl87Write(input:Word):string;
var
  r    :real;

begin
  r := swap(input);
  r := r / 210;
  str(r:3:1,Result);
  Result := Result + ' Volt';
end;

Function msWrite(input:Word):string;
var
  r    :real;

begin
  r := swap(input);
  r := r / 1250;    { 2000 for FFJ103, 1250 for FFJ104 }
  str(r:3:1,Result);
  Result := Result + ' ms';
end;

Function FTWrite(input:Word):string;
var
  r    :real;

begin
  r := swap(input);
  r := 120 - r * 0.75;
  if r > 0 then
  begin
    str(r:3:1,Result);
    Result := Result + ' btdc';
  end else
  begin
    str(-r:3:1,Result);
    Result := Result + ' atdc';
  end;
end;

Function DegWrite(input:Word):string;
var
  r    :real;

begin
  r := swap(input);
  r := r * 0.75;
  str(r:3:1,Result);
  result := Result +' deg';
end;

Function PctWrite(input:Word):string;
var
  r    :real;

begin
  r := swap(input);
  r := r * 100/65536;
  str(r:3:0,Result);
  result := Result +' %';
end;

Function PctIdleWrite(input:smallint):string;
var
  r    :real;

begin
  r := swap(input);
  r := r * 100/15000;
  str(r:3:0,Result);
  result := Result +' %';
end;

Function GWrite(input:smallint):string;
var
  r    :real;

begin
  r := swap(input);
  r := r / 16384;
  str(r:6:3,Result);
  result := Result +' G';
end;

Function LoadWrite(input:Word):string;
var
  r    :real;

begin
  r := swap(input);
  r := r * 100/3840;
  str(r:3:0,Result);
  result := Result +' %';
end;

Function AirMassWrite(input:Word):string;
var
  r    :real;

begin
  r := swap(input);
  r := r / 10;
  str(r:3:1,Result);
  result := Result +' kg/hour';
end;

Function MultWrite(input:Word):string;
var
  r    :real;

begin
  r := swap(input);
  r := r / 32768;
  r := (r - 1) * 100;
  str(r:3:0,Result);
  Result := Result + ' %';
end;

function FrqWrite(input:Word):string;
var
  r    :real;
  w    :word;

begin
  w := swap(input);
  r := w;
  r := r*20000000/16/65536;
  str(r:6:0,Result);
  Result := Result + ' Hz';
end;

function TimeConvert(input:longword):double;
var
  i      :byte;
  r      :double;
  liswap :array[0..3] of byte;
  lw     :longword absolute liswap;

begin
  for i := 0 to 3 do
  begin
    liswap[3-i] := input and 255;
    input := input shr 8;
  end;

  r := lw;
  result := (r/1000000);
end;

function TimeWrite(input:longword):string;
var
  i      :byte;
  r      :real;
  liswap :array[0..3] of byte;
  lw     :longword absolute liswap;

begin
  for i := 0 to 3 do
  begin
    liswap[3-i] := input and 255;
    input := input shr 8;
  end;

  r := lw;
  r := r/1000000;
  str(r:6:1,Result);
  Result := Result + ' sec';
end;

function wegfahrwrite(input:byte):string;
begin
  case input of
    1:result := 'On-CAN';
    2:result := 'On-Ohm';
  else
    result := 'NA/Off';
  end;
end;

function TractionOnOffwrite(input:byte):string;
begin
  case input of
    1:result := 'Speeds';
    2:result := 'VW Abs';
  else
    result := 'Off';
  end;
end;


Function OnOffWrite(input:byte; bit:byte):string;
begin
  if input and (1 shl bit) <> 0 then
    Result := 'On'
  else
    Result := 'Off';
end;

Procedure TINJStat.AddLoggerData(ByteLen :integer);
begin
  if ByteLen = 0 then
  begin
    LogIndex := 0;
    exit;
  end;

  while (Bytelen >= (LogIndex+1) * sizeof(TStatus)) do
  begin
    with UploadLog[LogIndex] do
    begin
      CurrTime := TimeConvert(StatusTime);
      if LogIndex = 0 then
      begin
        Time := 0;
        PrevTime := CurrTime;
      end;
      if (CurrTime<PrevTime) or (CurrTime-PrevTime > 10) then PrevTime := CurrTime-10;

      Time := time + (CurrTime-PrevTime);
      PrevTime := CurrTime;
    end;
    inc(LogIndex);
  end;
end;


Procedure TINJStat.ShowStatus;
begin
  with statusData do
  begin
    WatertempLabel.caption := Tempwrite(Water_temperature);
    Airtemplabel.caption := Tempwrite(Air_Temperature);
    AirMassVoltlabel.caption := Volt7write(AirMass_volt);
    Potmeterlabel.caption := Volt5Write(Potmeter);
    EgasFeedBacklabel.caption := Volt5Write(StatusEgasFeedback);
    Idlelabel.caption := onoffWrite(MotorFlags,4);
    Powershift.caption := onoffwrite(outbits,6);
    WideOpenTrottlelabel.caption := onoffWrite(MotorFlags,5);
    MapSensorlabel.caption := Volt5Write(Map_Sensor);
    KL87Voltagelabel.caption := kl87write(KL87_Voltage);
    LambdaVoltagelabel.caption := Lambdawrite(Lambda_Voltage);
    RPMlabel.caption := RPMWrite(RPM);
    IntakeCamAdjLabel.caption := CamAngleWrite(CamAngle1);
    ExhaustCamAdjlabel.caption := CamAngleWrite(Camangle2);
    IntakeCamRawLabel.caption := CamAngleWrite(StatusCamTrigPoint1);
    ExhaustCamRawlabel.caption := CamAngleWrite(StatusCamTrigPoint2);
    KnockSensor1label.caption := Swapwrite(Knock_Sensor_1);
    KnockSensor2label.caption := Swapwrite(Knock_Sensor_2);
    RoadSpeedlabel.caption := Speedwrite(Road_Speed);
    WegFahrSperrelabel.caption := WegFahrwrite(Weg_Fahr_Sperre);
    Tractionlabel.caption := Tractiononoffwrite(Traction_Flags);
    Antilaglabel.caption := onoffWrite(ALS_Flags,0);
    KL15label.caption := onoffWrite(Outbits,1);
    GXLabel.Caption := GWrite(GSensorX);
    GYLabel.Caption := GWrite(GSensorY);
    GZLabel.Caption := GWrite(GSensorZ);

    OtErrorlabel.caption := Swapwrite(Trigger_Error);
    Otsynclabel.caption := OnOffwrite(motorflags,0);
    camsynclabel.caption := OnOffwrite(motorflags,1);
    loadlabel.caption := LoadWrite(Load);
    airmassflowlabel.caption := AirMasswrite(Airmass_Flow);
    injectortimelabel.caption := MsWrite(Injector_Time);
    ignitiontimelabel.caption := FTwrite(Ignition_Time);
    dwelllabel.caption := Degwrite(Dwell_on_Time);
    LambdaPowerPWMLabel.caption := Pctwrite(StatusLambdaPWM);
    LambdaZLabel.caption := frqWrite(LambdaZ);
    undrivLlabel.caption := speedwrite(WheelSpeed_UD_L);
    undrivRlabel.caption := speedwrite(WheelSpeed_UD_R);
    IntakeVanosPWMLabel.caption := Pctwrite(PWMVanos1);
    ExhaustVanosPWMLabel.caption := Pctwrite(PWMVanos2);
    Tisignallabel.caption := PctWrite(PWMTI);
    Pwmidlelabel.caption := PctIdleWrite(PWMIdle);
    Errorlamplabel.caption := onoffWrite(Outbits,5);
    mainrelaylabel.caption := onoffwrite(Outbits,0);
    fuelpumplabel.caption := onoffwrite(outbits,2);
    evaporationlabel.caption := onoffwrite(outbits,4);
    chokerMultlabel.caption := multwrite(StatusChokerMult);
    chokerAddlabel.caption := mswrite(StatusChokerAdd);
    TractionRegulatelabel.caption := Degwrite(StatusTraction);
    LapCountLabel.caption := SwapWrite(StatusLapCount);
    LapTimeLabel.caption := TimeWrite(StatusLapTime);
    StatusTimeLabel.caption := TimeWrite(StatusTime);
  end;
  H8Com.COMRx(@StatusData, Sizeof(statusData), COMTimeout, InterCharTimeout);
end;


{$R *.DFM}

procedure TINJStat.Simu1000Click(Sender: TObject);
var
  rpm         :real;
  simuspeed   :word;
  s           :shortstring;

begin
  rpm := 1000;

  if (rpm > 91) and (rpm < 10000) then
  begin
    rpm := rpm/60;  { into hz }
    rpm := 1/rpm;   { revolution time in sec }
    rpm := rpm/60;  { time between interrupts }
    simuspeed := round(rpm*2000000);
  end else simuspeed := 0;

  s := MainProgCommand + MainProgSimulatorCmd;
  s := s + char(hi(simuspeed));
  s := s + char(lo(simuspeed));
  s := s + #0;
  s := s + #0;

  H8Com.SendCommandToCPUProtected(s);
end;

procedure TINJStat.SimuOffClick(Sender: TObject);
var
  s  :shortstring;

begin
  s := MainProgCommand + MainProgSimulatorCmd;
  s := s + #0;
  s := s + #0;
  s := s + #0;
  s := s + #0;

  H8Com.SendCommandToCPUProtected(s);
end;

procedure TINJStat.Simu100Click(Sender: TObject);
var
  rpm         :real;
  simuspeed   :word;
  s           :shortstring;

begin
  rpm := 100;

  if (rpm > 91) and (rpm < 10000) then
  begin
    rpm := rpm/60;  { into hz }
    rpm := 1/rpm;   { revolution time in sec }
    rpm := rpm/60;  { time between interrupts }
    simuspeed := round(rpm*2000000);
  end else simuspeed := 0;

  s := MainProgCommand + MainProgSimulatorCmd;
  s := s + char(hi(simuspeed));
  s := s + char(lo(simuspeed));
  s := s + #0;
  s := s + #0;

  H8Com.SendCommandToCPUProtected(s);
end;

end.
