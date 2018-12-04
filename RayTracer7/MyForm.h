#pragma once

//using namespace std;
#include <string>
#include <opencv2\opencv.hpp>
//#include "Renderer.h"
#include "Renderer.h"

using namespace cv;

namespace RayTracer7 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{

	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			renderer = new Renderer();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  lblCameraPosition;
	protected:

	protected:

	private: System::Windows::Forms::Button^  testButton;
	private: System::Windows::Forms::Button^  btnMorePositionX;

	private: System::Windows::Forms::Label^  lblCameraPositionX;
	private: System::Windows::Forms::Label^  lblCameraPositionY;
	private: System::Windows::Forms::Label^  lblCameraPositionZ;
	private: System::Windows::Forms::TextBox^  txtCameraPositionX;
	private: System::Windows::Forms::TextBox^  txtCameraPositionZ;
	private: System::Windows::Forms::TextBox^  txtCameraPositionY;
	private: System::Windows::Forms::Button^  btnLessPositionX;
	private: System::Windows::Forms::Button^  btnLessPositionZ;
	private: System::Windows::Forms::Button^  btnMorePositionZ;
	private: System::Windows::Forms::Button^  btnLessPositionY;
	private: System::Windows::Forms::Button^  btnMorePositionY;

	private: Renderer* renderer;
	private: System::Windows::Forms::Button^  btnLessDirectionY;

	private: System::Windows::Forms::Button^  btnMoreDirectionY;




	private: System::Windows::Forms::Button^  btnLessDirectionX;


	private: System::Windows::Forms::TextBox^  txtCameraDirectionY;



	private: System::Windows::Forms::TextBox^  txtCameraDirectionX;


	private: System::Windows::Forms::Label^  lblCameraDirectionY;
	private: System::Windows::Forms::Label^  lblCameraDirectionX;
	private: System::Windows::Forms::Button^  btnMoreDirectionX;

	private: System::Windows::Forms::Label^  lblCameraDirection;
	private: System::Windows::Forms::Label^  lblAspectRatio;
	private: System::Windows::Forms::TextBox^  txtHeight;
	private: System::Windows::Forms::TextBox^  txtWidth;
	private: System::Windows::Forms::Label^  lblHeight;
	private: System::Windows::Forms::Label^  lblWidth;



	protected:


	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->lblCameraPosition = (gcnew System::Windows::Forms::Label());
			this->testButton = (gcnew System::Windows::Forms::Button());
			this->btnMorePositionX = (gcnew System::Windows::Forms::Button());
			this->lblCameraPositionX = (gcnew System::Windows::Forms::Label());
			this->lblCameraPositionY = (gcnew System::Windows::Forms::Label());
			this->lblCameraPositionZ = (gcnew System::Windows::Forms::Label());
			this->txtCameraPositionX = (gcnew System::Windows::Forms::TextBox());
			this->txtCameraPositionZ = (gcnew System::Windows::Forms::TextBox());
			this->txtCameraPositionY = (gcnew System::Windows::Forms::TextBox());
			this->btnLessPositionX = (gcnew System::Windows::Forms::Button());
			this->btnLessPositionZ = (gcnew System::Windows::Forms::Button());
			this->btnMorePositionZ = (gcnew System::Windows::Forms::Button());
			this->btnLessPositionY = (gcnew System::Windows::Forms::Button());
			this->btnMorePositionY = (gcnew System::Windows::Forms::Button());
			this->btnLessDirectionY = (gcnew System::Windows::Forms::Button());
			this->btnMoreDirectionY = (gcnew System::Windows::Forms::Button());
			this->btnLessDirectionX = (gcnew System::Windows::Forms::Button());
			this->txtCameraDirectionY = (gcnew System::Windows::Forms::TextBox());
			this->txtCameraDirectionX = (gcnew System::Windows::Forms::TextBox());
			this->lblCameraDirectionY = (gcnew System::Windows::Forms::Label());
			this->lblCameraDirectionX = (gcnew System::Windows::Forms::Label());
			this->btnMoreDirectionX = (gcnew System::Windows::Forms::Button());
			this->lblCameraDirection = (gcnew System::Windows::Forms::Label());
			this->lblAspectRatio = (gcnew System::Windows::Forms::Label());
			this->txtHeight = (gcnew System::Windows::Forms::TextBox());
			this->txtWidth = (gcnew System::Windows::Forms::TextBox());
			this->lblHeight = (gcnew System::Windows::Forms::Label());
			this->lblWidth = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// lblCameraPosition
			// 
			this->lblCameraPosition->AutoSize = true;
			this->lblCameraPosition->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblCameraPosition->Location = System::Drawing::Point(102, 59);
			this->lblCameraPosition->Name = L"lblCameraPosition";
			this->lblCameraPosition->Size = System::Drawing::Size(126, 17);
			this->lblCameraPosition->TabIndex = 0;
			this->lblCameraPosition->Text = L"Camera Position";
			// 
			// testButton
			// 
			this->testButton->Location = System::Drawing::Point(547, 225);
			this->testButton->Name = L"testButton";
			this->testButton->Size = System::Drawing::Size(150, 23);
			this->testButton->TabIndex = 1;
			this->testButton->Text = L"Render";
			this->testButton->UseVisualStyleBackColor = true;
			this->testButton->Click += gcnew System::EventHandler(this, &MyForm::testButton_Click);
			// 
			// btnMorePositionX
			// 
			this->btnMorePositionX->Location = System::Drawing::Point(317, 91);
			this->btnMorePositionX->Name = L"btnMorePositionX";
			this->btnMorePositionX->Size = System::Drawing::Size(27, 23);
			this->btnMorePositionX->TabIndex = 2;
			this->btnMorePositionX->Text = L"+";
			this->btnMorePositionX->UseVisualStyleBackColor = true;
			this->btnMorePositionX->Click += gcnew System::EventHandler(this, &MyForm::btnMorePositionX_Click);
			// 
			// lblCameraPositionX
			// 
			this->lblCameraPositionX->AutoSize = true;
			this->lblCameraPositionX->Location = System::Drawing::Point(207, 92);
			this->lblCameraPositionX->Name = L"lblCameraPositionX";
			this->lblCameraPositionX->Size = System::Drawing::Size(21, 17);
			this->lblCameraPositionX->TabIndex = 3;
			this->lblCameraPositionX->Text = L"X:";
			// 
			// lblCameraPositionY
			// 
			this->lblCameraPositionY->AutoSize = true;
			this->lblCameraPositionY->Location = System::Drawing::Point(207, 122);
			this->lblCameraPositionY->Name = L"lblCameraPositionY";
			this->lblCameraPositionY->Size = System::Drawing::Size(21, 17);
			this->lblCameraPositionY->TabIndex = 4;
			this->lblCameraPositionY->Text = L"Y:";
			// 
			// lblCameraPositionZ
			// 
			this->lblCameraPositionZ->AutoSize = true;
			this->lblCameraPositionZ->Location = System::Drawing::Point(207, 149);
			this->lblCameraPositionZ->Name = L"lblCameraPositionZ";
			this->lblCameraPositionZ->Size = System::Drawing::Size(21, 17);
			this->lblCameraPositionZ->TabIndex = 5;
			this->lblCameraPositionZ->Text = L"Z:";
			// 
			// txtCameraPositionX
			// 
			this->txtCameraPositionX->Location = System::Drawing::Point(250, 92);
			this->txtCameraPositionX->Name = L"txtCameraPositionX";
			this->txtCameraPositionX->Size = System::Drawing::Size(61, 22);
			this->txtCameraPositionX->TabIndex = 6;
			this->txtCameraPositionX->Text = L"0";
			// 
			// txtCameraPositionZ
			// 
			this->txtCameraPositionZ->Location = System::Drawing::Point(250, 150);
			this->txtCameraPositionZ->Name = L"txtCameraPositionZ";
			this->txtCameraPositionZ->Size = System::Drawing::Size(61, 22);
			this->txtCameraPositionZ->TabIndex = 7;
			this->txtCameraPositionZ->Text = L"-4.8";
			this->txtCameraPositionZ->TextChanged += gcnew System::EventHandler(this, &MyForm::txtCameraPositionZ_TextChanged);
			// 
			// txtCameraPositionY
			// 
			this->txtCameraPositionY->Location = System::Drawing::Point(250, 122);
			this->txtCameraPositionY->Name = L"txtCameraPositionY";
			this->txtCameraPositionY->Size = System::Drawing::Size(61, 22);
			this->txtCameraPositionY->TabIndex = 8;
			this->txtCameraPositionY->Text = L"0.5";
			// 
			// btnLessPositionX
			// 
			this->btnLessPositionX->Location = System::Drawing::Point(350, 91);
			this->btnLessPositionX->Name = L"btnLessPositionX";
			this->btnLessPositionX->Size = System::Drawing::Size(27, 23);
			this->btnLessPositionX->TabIndex = 9;
			this->btnLessPositionX->Text = L"-";
			this->btnLessPositionX->UseVisualStyleBackColor = true;
			this->btnLessPositionX->Click += gcnew System::EventHandler(this, &MyForm::btnLessPositionX_Click);
			// 
			// btnLessPositionZ
			// 
			this->btnLessPositionZ->Location = System::Drawing::Point(350, 150);
			this->btnLessPositionZ->Name = L"btnLessPositionZ";
			this->btnLessPositionZ->Size = System::Drawing::Size(27, 23);
			this->btnLessPositionZ->TabIndex = 11;
			this->btnLessPositionZ->Text = L"-";
			this->btnLessPositionZ->UseVisualStyleBackColor = true;
			this->btnLessPositionZ->Click += gcnew System::EventHandler(this, &MyForm::btnLessPositionZ_Click);
			// 
			// btnMorePositionZ
			// 
			this->btnMorePositionZ->Location = System::Drawing::Point(317, 150);
			this->btnMorePositionZ->Name = L"btnMorePositionZ";
			this->btnMorePositionZ->Size = System::Drawing::Size(27, 23);
			this->btnMorePositionZ->TabIndex = 10;
			this->btnMorePositionZ->Text = L"+";
			this->btnMorePositionZ->UseVisualStyleBackColor = true;
			this->btnMorePositionZ->Click += gcnew System::EventHandler(this, &MyForm::btnMorePositionZ_Click);
			// 
			// btnLessPositionY
			// 
			this->btnLessPositionY->Location = System::Drawing::Point(350, 122);
			this->btnLessPositionY->Name = L"btnLessPositionY";
			this->btnLessPositionY->Size = System::Drawing::Size(27, 23);
			this->btnLessPositionY->TabIndex = 13;
			this->btnLessPositionY->Text = L"-";
			this->btnLessPositionY->UseVisualStyleBackColor = true;
			this->btnLessPositionY->Click += gcnew System::EventHandler(this, &MyForm::btnLessPositionY_Click);
			// 
			// btnMorePositionY
			// 
			this->btnMorePositionY->Location = System::Drawing::Point(317, 122);
			this->btnMorePositionY->Name = L"btnMorePositionY";
			this->btnMorePositionY->Size = System::Drawing::Size(27, 23);
			this->btnMorePositionY->TabIndex = 12;
			this->btnMorePositionY->Text = L"+";
			this->btnMorePositionY->UseVisualStyleBackColor = true;
			this->btnMorePositionY->Click += gcnew System::EventHandler(this, &MyForm::btnMorePositionY_Click);
			// 
			// btnLessDirectionY
			// 
			this->btnLessDirectionY->Location = System::Drawing::Point(367, 256);
			this->btnLessDirectionY->Name = L"btnLessDirectionY";
			this->btnLessDirectionY->Size = System::Drawing::Size(61, 24);
			this->btnLessDirectionY->TabIndex = 26;
			this->btnLessDirectionY->Text = L"Right";
			this->btnLessDirectionY->UseVisualStyleBackColor = true;
			this->btnLessDirectionY->Click += gcnew System::EventHandler(this, &MyForm::btnLessDirectionY_Click);
			// 
			// btnMoreDirectionY
			// 
			this->btnMoreDirectionY->Location = System::Drawing::Point(317, 255);
			this->btnMoreDirectionY->Name = L"btnMoreDirectionY";
			this->btnMoreDirectionY->Size = System::Drawing::Size(44, 25);
			this->btnMoreDirectionY->TabIndex = 25;
			this->btnMoreDirectionY->Text = L"Left";
			this->btnMoreDirectionY->UseVisualStyleBackColor = true;
			this->btnMoreDirectionY->Click += gcnew System::EventHandler(this, &MyForm::btnMoreDirectionY_Click);
			// 
			// btnLessDirectionX
			// 
			this->btnLessDirectionX->Location = System::Drawing::Point(367, 224);
			this->btnLessDirectionX->Name = L"btnLessDirectionX";
			this->btnLessDirectionX->Size = System::Drawing::Size(61, 25);
			this->btnLessDirectionX->TabIndex = 22;
			this->btnLessDirectionX->Text = L"Down";
			this->btnLessDirectionX->UseVisualStyleBackColor = true;
			this->btnLessDirectionX->Click += gcnew System::EventHandler(this, &MyForm::btnLessDirectionX_Click);
			// 
			// txtCameraDirectionY
			// 
			this->txtCameraDirectionY->Location = System::Drawing::Point(250, 255);
			this->txtCameraDirectionY->Name = L"txtCameraDirectionY";
			this->txtCameraDirectionY->Size = System::Drawing::Size(61, 22);
			this->txtCameraDirectionY->TabIndex = 21;
			this->txtCameraDirectionY->Text = L"0";
			// 
			// txtCameraDirectionX
			// 
			this->txtCameraDirectionX->Location = System::Drawing::Point(250, 225);
			this->txtCameraDirectionX->Name = L"txtCameraDirectionX";
			this->txtCameraDirectionX->Size = System::Drawing::Size(61, 22);
			this->txtCameraDirectionX->TabIndex = 19;
			this->txtCameraDirectionX->Text = L"0";
			// 
			// lblCameraDirectionY
			// 
			this->lblCameraDirectionY->AutoSize = true;
			this->lblCameraDirectionY->Location = System::Drawing::Point(154, 255);
			this->lblCameraDirectionY->Name = L"lblCameraDirectionY";
			this->lblCameraDirectionY->Size = System::Drawing::Size(74, 17);
			this->lblCameraDirectionY->TabIndex = 17;
			this->lblCameraDirectionY->Text = L"Left-Right:";
			// 
			// lblCameraDirectionX
			// 
			this->lblCameraDirectionX->AutoSize = true;
			this->lblCameraDirectionX->Location = System::Drawing::Point(158, 224);
			this->lblCameraDirectionX->Name = L"lblCameraDirectionX";
			this->lblCameraDirectionX->Size = System::Drawing::Size(70, 17);
			this->lblCameraDirectionX->TabIndex = 16;
			this->lblCameraDirectionX->Text = L"Up-Down:";
			// 
			// btnMoreDirectionX
			// 
			this->btnMoreDirectionX->Location = System::Drawing::Point(317, 224);
			this->btnMoreDirectionX->Name = L"btnMoreDirectionX";
			this->btnMoreDirectionX->Size = System::Drawing::Size(44, 25);
			this->btnMoreDirectionX->TabIndex = 15;
			this->btnMoreDirectionX->Text = L"Up";
			this->btnMoreDirectionX->UseVisualStyleBackColor = true;
			this->btnMoreDirectionX->Click += gcnew System::EventHandler(this, &MyForm::btnMoreDirectionX_Click);
			// 
			// lblCameraDirection
			// 
			this->lblCameraDirection->AutoSize = true;
			this->lblCameraDirection->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblCameraDirection->Location = System::Drawing::Point(95, 192);
			this->lblCameraDirection->Name = L"lblCameraDirection";
			this->lblCameraDirection->Size = System::Drawing::Size(133, 17);
			this->lblCameraDirection->TabIndex = 14;
			this->lblCameraDirection->Text = L"Camera Direction";
			// 
			// lblAspectRatio
			// 
			this->lblAspectRatio->AutoSize = true;
			this->lblAspectRatio->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblAspectRatio->Location = System::Drawing::Point(14, 312);
			this->lblAspectRatio->Name = L"lblAspectRatio";
			this->lblAspectRatio->Size = System::Drawing::Size(214, 17);
			this->lblAspectRatio->TabIndex = 27;
			this->lblAspectRatio->Text = L"Aspect Ratio (Only Integers)";
			// 
			// txtHeight
			// 
			this->txtHeight->Location = System::Drawing::Point(250, 372);
			this->txtHeight->Name = L"txtHeight";
			this->txtHeight->Size = System::Drawing::Size(61, 22);
			this->txtHeight->TabIndex = 29;
			this->txtHeight->Text = L"480";
			// 
			// txtWidth
			// 
			this->txtWidth->Location = System::Drawing::Point(250, 342);
			this->txtWidth->Name = L"txtWidth";
			this->txtWidth->Size = System::Drawing::Size(61, 22);
			this->txtWidth->TabIndex = 28;
			this->txtWidth->Text = L"640";
			// 
			// lblHeight
			// 
			this->lblHeight->AutoSize = true;
			this->lblHeight->Location = System::Drawing::Point(175, 372);
			this->lblHeight->Name = L"lblHeight";
			this->lblHeight->Size = System::Drawing::Size(53, 17);
			this->lblHeight->TabIndex = 31;
			this->lblHeight->Text = L"Height:";
			// 
			// lblWidth
			// 
			this->lblWidth->AutoSize = true;
			this->lblWidth->Location = System::Drawing::Point(180, 342);
			this->lblWidth->Name = L"lblWidth";
			this->lblWidth->Size = System::Drawing::Size(48, 17);
			this->lblWidth->TabIndex = 30;
			this->lblWidth->Text = L"Width:";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(809, 455);
			this->Controls->Add(this->lblHeight);
			this->Controls->Add(this->lblWidth);
			this->Controls->Add(this->txtHeight);
			this->Controls->Add(this->txtWidth);
			this->Controls->Add(this->lblAspectRatio);
			this->Controls->Add(this->btnLessDirectionY);
			this->Controls->Add(this->btnMoreDirectionY);
			this->Controls->Add(this->btnLessDirectionX);
			this->Controls->Add(this->txtCameraDirectionY);
			this->Controls->Add(this->txtCameraDirectionX);
			this->Controls->Add(this->lblCameraDirectionY);
			this->Controls->Add(this->lblCameraDirectionX);
			this->Controls->Add(this->btnMoreDirectionX);
			this->Controls->Add(this->lblCameraDirection);
			this->Controls->Add(this->btnLessPositionY);
			this->Controls->Add(this->btnMorePositionY);
			this->Controls->Add(this->btnLessPositionZ);
			this->Controls->Add(this->btnMorePositionZ);
			this->Controls->Add(this->btnLessPositionX);
			this->Controls->Add(this->txtCameraPositionY);
			this->Controls->Add(this->txtCameraPositionZ);
			this->Controls->Add(this->txtCameraPositionX);
			this->Controls->Add(this->lblCameraPositionZ);
			this->Controls->Add(this->lblCameraPositionY);
			this->Controls->Add(this->lblCameraPositionX);
			this->Controls->Add(this->btnMorePositionX);
			this->Controls->Add(this->testButton);
			this->Controls->Add(this->lblCameraPosition);
			this->Name = L"MyForm";
			this->Text = L"Options";
			this->ResumeLayout(false);
			this->PerformLayout();

		}


		private: System::Void testButton_Click(System::Object^  sender, System::EventArgs^  e) {

			//label1->Text = "Rendering ...";

			renderer->setCamPos(System::Convert::ToDouble(txtCameraPositionX->Text), 
				System::Convert::ToDouble(txtCameraPositionY->Text), 
				System::Convert::ToDouble(txtCameraPositionZ->Text));
			renderer->setWidth(System::Convert::ToInt32(txtWidth->Text));
			renderer->setHeight(System::Convert::ToInt32(txtHeight->Text));
			renderer->setCamDir(System::Convert::ToDouble(txtCameraDirectionX->Text)*-1, System::Convert::ToDouble(txtCameraDirectionY->Text)*-1);
			std::string Something = "Some text";

			int ex = 5;
			System::String^ t = System::Convert::ToString(ex);
			testButton->Text = "Rendering ...";
			
			renderer->render();
			testButton->Enabled = false;
			
			Mat original = imread("scene.bmp", CV_LOAD_IMAGE_COLOR);

			imshow("Original", original);

			testButton->Enabled = true;
			testButton->Text = "Render";

		}
		
		



private: System::Void btnMorePositionX_Click(System::Object^  sender, System::EventArgs^  e) {

	double valueX = System::Convert::ToDouble(txtCameraPositionX->Text);
	valueX += 1;
	txtCameraPositionX->Text = System::Convert::ToString(valueX);

}
private: System::Void btnLessPositionX_Click(System::Object^  sender, System::EventArgs^  e) {

	double valueX = System::Convert::ToDouble(txtCameraPositionX->Text);
	valueX -= 1;
	txtCameraPositionX->Text = System::Convert::ToString(valueX);
	//delete &valueX;
}
private: System::Void btnMorePositionY_Click(System::Object^  sender, System::EventArgs^  e) {

	double valueY = System::Convert::ToDouble(txtCameraPositionY->Text);
	valueY += 1;
	txtCameraPositionY->Text = System::Convert::ToString(valueY);
}
private: System::Void btnLessPositionY_Click(System::Object^  sender, System::EventArgs^  e) {
	double valueY = System::Convert::ToDouble(txtCameraPositionY->Text);
	valueY -= 1;
	txtCameraPositionY->Text = System::Convert::ToString(valueY);

}
private: System::Void btnMorePositionZ_Click(System::Object^  sender, System::EventArgs^  e) {
	double valueZ = System::Convert::ToDouble(txtCameraPositionZ->Text);
	valueZ += 1;
	txtCameraPositionZ->Text = System::Convert::ToString(valueZ);

}
private: System::Void btnLessPositionZ_Click(System::Object^  sender, System::EventArgs^  e) {
	double valueZ = System::Convert::ToDouble(txtCameraPositionZ->Text);
	valueZ -= 1;
	txtCameraPositionZ->Text = System::Convert::ToString(valueZ);
}
private: System::Void txtCameraPositionZ_TextChanged(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void btnMoreDirectionX_Click(System::Object^  sender, System::EventArgs^  e) {
	double valueX = System::Convert::ToDouble(txtCameraDirectionX->Text);
	valueX += 1;
	txtCameraDirectionX->Text = System::Convert::ToString(valueX);


}
private: System::Void btnLessDirectionY_Click(System::Object^  sender, System::EventArgs^  e) {
	double valueY = System::Convert::ToDouble(txtCameraDirectionY->Text);
	valueY -= 1;
	txtCameraDirectionY->Text = System::Convert::ToString(valueY);
}
private: System::Void btnLessDirectionX_Click(System::Object^  sender, System::EventArgs^  e) {
	double valueX = System::Convert::ToDouble(txtCameraDirectionX->Text);
	valueX -= 1;
	txtCameraDirectionX->Text = System::Convert::ToString(valueX);

}
private: System::Void btnMoreDirectionY_Click(System::Object^  sender, System::EventArgs^  e) {
	double valueY = System::Convert::ToDouble(txtCameraDirectionY->Text);
	valueY += 1;
	txtCameraDirectionY->Text = System::Convert::ToString(valueY);

}
};
}