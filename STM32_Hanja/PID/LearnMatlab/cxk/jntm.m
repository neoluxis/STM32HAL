function jntm()
video = "tsk.mp4";
vidObj = VideoReader(video);

%读取总帧数
a=2;
nFrames = vidObj.NumFrames;
vFrameRate = vidObj.FrameRate;

[audio_input, fs] = audioread(video);
sound(audio_input, fs);


for i = 1 : nFrames
    
    t1=clock;
    frame = imread(fullfile('store\',[num2str(i) '.jpg']));
    image(frame);
    colormap(gray);
    t2=clock;
    pause(1/(vFrameRate)-1.01*(t2(6)-t1(6)));%调节延时
end

close;

end