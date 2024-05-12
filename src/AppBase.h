#ifndef _APP_BASE_H_
#define _APP_BASE_H_

class AppBase {
    public:
        virtual void Run() {
            Input();
            Update();
            Render();
        }

        bool Closed() 
        {
            return closed;
        }

        void Close()
        {
            closed = true;
        }

        virtual void Input() = 0;
        virtual void Update() = 0;
        virtual void Render() = 0;

    private:
        bool closed = false;
};

#endif