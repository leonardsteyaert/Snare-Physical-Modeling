#include "plugin.hpp"
#include "delay.hpp"
#include "OnePoleLPF.hpp"
#include "OnePoleHPF.hpp"
#include "WhiteNoise.hpp"
#include "AllPass.hpp"


struct ChairSnare1 : Module {


	

	enum ParamId {
		TRIG_SHAPE_KNOB_PARAM,
		D1_KNOB_PARAM,
		D5_KNOB_PARAM,
		THETA1_KNOB_PARAM,
		THETA4_KNOB_PARAM,
		EXTRA1_KNOB_PARAM,
		EXTRA2_KNOB_PARAM,
		EXTRA3_KNOB_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		TRIG_SHAPE_CV_INPUT,
		D1_CV_INPUT,
		D5_CV_INPUT,
		THETA1_CV_INPUT,
		THETA4_CV_INPUT,
		TRIG_CV_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUTPUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	OnePoleHPF input_hpf_1;
	OnePoleHPF input_hpf_2;
	OnePoleHPF input_hpf_3;
	OnePoleHPF input_hpf_4;

	OnePoleLPF DC_lpf;
	OnePoleLPF general_delay_time_lpf;

	OnePoleLPF feedback_loop_lpf;
	OnePoleHPF feedback_loop_hpf;

	Delay general_delay; // 8820 = 200ms * 44100Hz / 1000(ms/s)

	AllPass allpass_1;
	AllPass allpass_2;
	AllPass allpass_3;
	AllPass allpass_4;

	OnePoleLPF allpass_1_noise_delaytime_lpf;
	OnePoleLPF allpass_1_noise_theta_lpf;
	WhiteNoise allpass_1_noise_delaytime;
	WhiteNoise allpass_1_noise_theta;

    OnePoleLPF allpass_4_noise_theta_lpf;
	WhiteNoise allpass_4_noise_theta;


	ChairSnare1() : general_delay(8820), allpass_1(6.53), allpass_2(2.2), allpass_3(2.5), allpass_4(4){
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(TRIG_SHAPE_KNOB_PARAM, 0.f, 1.f, 0.f, "");
		configParam(D1_KNOB_PARAM, 0.f, 1.f, 0.f, "");
		configParam(D5_KNOB_PARAM, 0.f, 1.f, 0.f, "");
		configParam(THETA1_KNOB_PARAM, 0.f, 1.f, 0.f, "");
		configParam(THETA4_KNOB_PARAM, 0.f, 1.f, 0.f, "");
		configParam(EXTRA1_KNOB_PARAM, 0.f, 1.f, 0.f, "");
		configParam(EXTRA2_KNOB_PARAM, 0.f, 1.f, 0.f, "");
		configParam(EXTRA3_KNOB_PARAM, 0.f, 1.f, 0.f, "");
		configInput(TRIG_SHAPE_CV_INPUT, "");
		configInput(D1_CV_INPUT, "");
		configInput(D5_CV_INPUT, "");
		configInput(THETA1_CV_INPUT, "");
		configInput(THETA4_CV_INPUT, "");
		configInput(TRIG_CV_INPUT, "");
		configOutput(OUTPUT_OUTPUT, "");

	}

	void process(const ProcessArgs& args) override {

		/*
		
		OBSERVATIONS:

			- My LPF cuts high frequencies way more than the lop in PD, in the feedback loop
			- My Allpass filter, diverges (feedbacks) for theta = 0.5, so you can be near but not exactly 0.5 (unlinke in  PD)
		*/

		// Get Input
		float input_raw = inputs[D1_CV_INPUT].getVoltage();
	    
		// Remove DC
		/*input_hpf_1.setCutoff_hpf(4.f, args.sampleRate);
		input_hpf_2.setCutoff_hpf(4.f, args.sampleRate);
		input_hpf_3.setCutoff_hpf(4.f, args.sampleRate);
		input_hpf_4.setCutoff_hpf(4.f, args.sampleRate);
		float input = input_hpf_4.process_hpf(input_hpf_3.process_hpf(input_hpf_2.process_hpf(input_hpf_1.process_hpf(input_raw))));
		

		// Generate feedback input
	    float feedback_input = general_delay.read(6, args.sampleRate);

		// Allpass 2 : Compute
		float allpass_output_2 = allpass_2.process_allpass(feedback_input, 2.2f ,params[THETA4_KNOB_PARAM].getValue(), args.sampleRate);

		// Allpass 3 : Compute
		float allpass_output_3 = allpass_3.process_allpass(allpass_output_2, 2.5f ,params[THETA1_KNOB_PARAM].getValue(), args.sampleRate);

		// Feedback Loop Output Processing
		feedback_loop_lpf.setCutoff_lpf(15000.f, args.sampleRate);
		feedback_loop_hpf.setCutoff_hpf(200.f, args.sampleRate);
		float feedback_output = feedback_loop_hpf.process_hpf(feedback_loop_lpf.process_lpf(allpass_output_3)) * (-1.f) * 0.98f;


		// Add Input to Feedback System
		float output = input + feedback_output;

		// Write Output to Feedback System
		general_delay.write(output);*/

		// Lowpass 20
		feedback_loop_lpf.setCutoff_lpf(20.f, args.sampleRate);
		float output = feedback_loop_lpf.process_lpf(input_raw);

		// Generate Output
		outputs[OUTPUT_OUTPUT].setVoltage(output);	

		//------------------------------------
/*
		// Get Input
		float input_raw = inputs[D1_CV_INPUT].getVoltage();

		// Remove DC
		input_hpf_1.setCutoff_hpf(4.f, args.sampleRate);
		input_hpf_2.setCutoff_hpf(4.f, args.sampleRate);
		input_hpf_3.setCutoff_hpf(4.f, args.sampleRate);
		input_hpf_4.setCutoff_hpf(4.f, args.sampleRate);
		float input = input_hpf_4.process_hpf(input_hpf_3.process_hpf(input_hpf_2.process_hpf(input_hpf_1.process_hpf(input_raw))));

		// DC Offset amplified to be used as an evelope
		DC_lpf.setCutoff_lpf(100.f, args.sampleRate);
		float DC_offset = DC_lpf.process_lpf(input - input_raw) * 25.f;

		// Generate the right General Delay Time
		general_delay_time_lpf.setCutoff_lpf(20.f, args.sampleRate);
		float general_delay_time = general_delay_time_lpf.process_lpf(DC_offset) * 0.5f;

		// Generate feedback input
		float feedback_input = general_delay.read(general_delay_time, args.sampleRate);

		// Allpass 1 : Delay Time
		allpass_1_noise_delaytime_lpf.setCutoff_lpf(2000.f, args.sampleRate);
		float allpass_1_delay_time = allpass_1_noise_delaytime_lpf.process_lpf(allpass_1_noise_delaytime.generate())*0.5f;

		allpass_1_delay_time = 6.f;

		// Allpass 1 : Theta
		allpass_1_noise_theta_lpf.setCutoff_lpf(100.f, args.sampleRate);
		float allpass_1_theta = (allpass_1_noise_theta_lpf.process_lpf(allpass_1_noise_theta.generate())+1.f)*0.75f;

		// Allpass 1 : Compute
		float allpass_output_1 = allpass_1.process_allpass(feedback_input, allpass_1_delay_time,allpass_1_theta,args.sampleRate);

		// Allpass 2 : Compute
		float allpass_output_2 = allpass_2.process_allpass(allpass_output_1, 2.2f ,0.5f, args.sampleRate);

		// Allpass 3 : Compute
		float allpass_output_3 = allpass_3.process_allpass(allpass_output_2, 2.5f ,0.14f, args.sampleRate);

		// Allpass 4 : Theta
		allpass_4_noise_theta_lpf.setCutoff_lpf(500.f, args.sampleRate);
		float allpass_4_theta = allpass_4_noise_theta_lpf.process_lpf(allpass_4_noise_theta.generate())*0.01f;

		// Allpass 4 : Compute
		float allpass_output_4 = allpass_4.process_allpass(allpass_output_3, 4,allpass_4_theta,args.sampleRate);


		// Feedback Loop Output Processing
		feedback_loop_lpf.setCutoff_lpf(7000.f, args.sampleRate);
		feedback_loop_hpf.setCutoff_hpf(100.f, args.sampleRate);
		float feedback_output = feedback_loop_hpf.process_hpf(feedback_loop_lpf.process_lpf(allpass_output_4)) * (-1.f) * 0.01f; //0.98f

		// Add Input to Feedback System
		float output = input + feedback_output;

		// Write Output to Feedback System
		general_delay.write(output);

		// Generate Output
		outputs[OUTPUT_OUTPUT].setVoltage(output);			
*/
	}
};


struct ChairSnare1Widget : ModuleWidget {
	ChairSnare1Widget(ChairSnare1* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/ChairSnare1.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.065, 16.43)), module, ChairSnare1::TRIG_SHAPE_KNOB_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(31.115, 16.43)), module, ChairSnare1::D1_KNOB_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(48.354, 16.62)), module, ChairSnare1::D5_KNOB_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.269, 30.763)), module, ChairSnare1::THETA1_KNOB_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(31.319, 30.763)), module, ChairSnare1::THETA4_KNOB_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.542, 50.177)), module, ChairSnare1::EXTRA1_KNOB_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(31.592, 50.177)), module, ChairSnare1::EXTRA2_KNOB_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(48.831, 50.368)), module, ChairSnare1::EXTRA3_KNOB_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.065, 77.478)), module, ChairSnare1::TRIG_SHAPE_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(31.644, 77.478)), module, ChairSnare1::D1_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(49.172, 77.775)), module, ChairSnare1::D5_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.182, 91.257)), module, ChairSnare1::THETA1_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(31.762, 91.257)), module, ChairSnare1::THETA4_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.182, 104.621)), module, ChairSnare1::TRIG_CV_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(31.65, 117.254)), module, ChairSnare1::OUTPUT_OUTPUT));
	}
};


Model* modelChairSnare1 = createModel<ChairSnare1, ChairSnare1Widget>("ChairSnare1");