function varargout = SpaceLocator(varargin)
% SPACELOCATOR MATLAB code for SpaceLocator.fig
% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @SpaceLocator_OpeningFcn, ...
                   'gui_OutputFcn',  @SpaceLocator_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end

% End initialization code - DO NOT EDIT


% --- Executes just before SpaceLocator is made visible.
function SpaceLocator_OpeningFcn(hObject, ~, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to SpaceLocator (see VARARGIN)

% Choose default command line output for SpaceLocator
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);
plot3(0,0,0);
axis([0 5 0 5 0 5]);
set(gca,'Xtick',[0:0.5:5]);
set(gca,'Ytick',[0:0.5:5]);
set(gca,'Ztick',[0:0.5:5]);
set(gca, 'Xcolor', [0 0 0]);
set(gca, 'Ycolor', [1 0 0]);
set(gca, 'Zcolor', [0 0 1]);
grid on;
hold on;

% UIWAIT makes SpaceLocator wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = SpaceLocator_OutputFcn(~, ~, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;
global puase;
puase = 0;


% --- Executes on button press in ON.
function ON_Callback(~, ~, ~)
% hObject    handle to ON (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global puase stopFlag offCnt;

offCnt = 0;

puase = ~puase;
stopFlag = 1;
 for i=0:0.05:5
      if puase == 0
          break;
      end
      plot3(abs(sin(i)),abs(cos(i)),i*abs(sin(i)),'.');
      pause(1/10);
      if stopFlag == 0
          break;
      end
  end

% --- Executes on button press in OFF.
function OFF_Callback(~, ~, ~)
% hObject    handle to OFF (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global stopFlag offCnt puase;

offCnt = offCnt + 1;
if offCnt == 1
    stopFlag = 0;
    puase = 1;
    cla;         
elseif offCnt == 2    
     stopFlag = 0;
     close all;clc;clear all;
end



function disp_Callback(~, ~, ~)
% hObject    handle to disp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of disp as text
%        str2double(get(hObject,'String')) returns contents of disp as a double
