package biz.bruenn.WyLight;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class CopyHelper {
	private final String mOutputDirectory;

	CopyHelper(String outputDirectory) {
		mOutputDirectory = outputDirectory + '/';
	}

	public String copyBufferToFile(byte[] buffer, int bufferLength, String name) throws IOException {
		String outputFilename = mOutputDirectory + name;
		FileOutputStream out;
		out = new FileOutputStream(new File(outputFilename));
		out.write(buffer, 0, bufferLength);
		out.close();
		return outputFilename;
	}

	public String copyStreamToFile(InputStream input, String name) throws IOException {
		byte[] firmwareBuffer = new byte[1024];
		String outputFilename = mOutputDirectory + name;
		FileOutputStream out;
		out = new FileOutputStream(new File(outputFilename));
		for(int bytesRead = input.read(firmwareBuffer); bytesRead > 0; bytesRead = input.read(firmwareBuffer)) {
			System.out.print(firmwareBuffer);
			out.write(firmwareBuffer);
		}
		input.close();
		out.close();
		return outputFilename;
	}
}
